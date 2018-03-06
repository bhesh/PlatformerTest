/**
	@author Brian Hession

	Test animation
*/

#include "stdafx.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>

#include <iostream>
#include <stdio.h>

#include "plt/world/PltWindow.h"
#include "plt/world/World.h"

// Set debug mode
float DEBUG = false;

// Screen res
unsigned int WINDOW_WIDTH = 32 * 24;
unsigned int WINDOW_HEIGHT = 32 * 18;

// Scale
float SCALE = 2.f;

// Main entry
int main(int argc, char *argv[]) {

	// Load font
	sf::String fontFilename = L"fonts/monobit.ttf";
	sf::Font font;
	if (!font.loadFromFile(fontFilename)) {
		std::cerr << "Error loading font" << std::endl;
		return 1;
	}

	// Load tile texture
	sf::String tilesFilename = L"sprites/tiles.png";
	sf::Texture tiles;
	if (!tiles.loadFromFile(tilesFilename)) {
		std::cerr << "Error loading tiles" << std::endl;
		return 1;
	}
	tiles.setSmooth(false);
	
	// Make box sprite
	sf::Sprite boxSprite;
	boxSprite.setTexture(tiles);
	boxSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	boxSprite.setOrigin(16.f, 16.f);
	boxSprite.setScale(SCALE, SCALE);

	// Make metal sprite
	sf::Sprite metalSprite;
	metalSprite.setTexture(tiles);
	metalSprite.setTextureRect(sf::IntRect(32, 16, 32, 16));
	metalSprite.setOrigin(16.f, 8.f);
	metalSprite.setScale(SCALE, SCALE);

	// Make character sprite
	sf::RectangleShape characterSprite = sf::RectangleShape( sf::Vector2f(32.f, 64.f));
	characterSprite.setFillColor(sf::Color::White);
	characterSprite.setOrigin(16.f, 32.f);
	characterSprite.setScale(SCALE, SCALE);

	// Make the window
	plt::world::PltWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), 
		"Platformer Test", sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	window.setFont(&font); // For FPS
	window.showFps(DEBUG);

	// Make the world
	plt::world::World world(sf::Vector2f(0, 50.f));

	// Keep track of platforms and spawns
	std::vector<b2Body *> platforms;
	std::vector<b2Body *> spawns;

	// Create platforms
	b2Body *floor = world.CreateRect(sf::FloatRect(
		0,
		static_cast<float>(WINDOW_HEIGHT - (16.f * SCALE)),
		static_cast<float>(WINDOW_WIDTH),
		16.f * SCALE
	));
	platforms.push_back(floor);
	b2Body *platform = world.CreateRect(sf::FloatRect(
		static_cast<float>(WINDOW_WIDTH / 2),
		static_cast<float>(WINDOW_HEIGHT / 2),
		static_cast<float>(WINDOW_WIDTH / 2),
		16.f * SCALE
	));
	platforms.push_back(platform);

	// Create character
	b2Body *character = world.CreateRect(sf::FloatRect(
		10.f,
		200.f,
		32.f * SCALE,
		64.f * SCALE
	), b2_dynamicBody);
	character->SetFixedRotation(true);

	// Game loop
	sf::Clock clock;
	while (window.isOpen()) {

		// Get the delta time
		sf::Time dt = clock.restart();

		// Poll events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			/*else if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f pos = window.mapPixelToCoords(mousePos);
				b2Body *spawn = world.CreateRect(sf::FloatRect(
					pos.x - (16 * SCALE),
					pos.y - (16 * SCALE),
					32 * SCALE,
					32 * SCALE
				), b2_dynamicBody);
				spawns.push_back(spawn);
			}*/
			else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Space:
					/*for (auto it = spawns.begin(); it != spawns.end(); ++it)
						world.DestroyBody(*it);
					spawns.clear();*/
					character->ApplyLinearImpulse(b2Vec2(0, -150.f * SCALE),
						character->GetWorldCenter(), true);
					break;
				case sf::Keyboard::Tilde:
					DEBUG = !DEBUG;
					window.showFps(DEBUG);
					break;
				}
			}
		}

		// Get input
		b2Vec2 vel = character->GetLinearVelocity();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			character->SetLinearVelocity(b2Vec2(-6.f * SCALE, vel.y));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			character->SetLinearVelocity(b2Vec2(6.f * SCALE, vel.y));
		}
		else {
			character->SetLinearVelocity(b2Vec2(0, vel.y));
		}
 
		// Clear the screen
		window.clear();
		window.resetView();

		// Step the world
		world.Step(dt.asSeconds());

		// Draw sprites
		for (auto it = platforms.begin(); it != platforms.end(); ++it) {
			b2AABB bounds;
			(*it)->GetFixtureList()->GetShape()->ComputeAABB(&bounds, (*it)->GetTransform(), 0);
			sf::Vector2f lowerBounds = world.GetWindowPoint(bounds.lowerBound);
			sf::Vector2f upperBounds = world.GetWindowPoint(bounds.upperBound);
			for (float x = bounds.lowerBound.x; x < bounds.upperBound.x; x += (32.f * SCALE) / world.GetPixelScale()) {
				sf::Vector2f pos = world.GetWindowPoint(b2Vec2(x, bounds.lowerBound.y));
				pos += sf::Vector2f(16.f * SCALE, 8.f * SCALE);
				metalSprite.setPosition(pos);
				window.draw(metalSprite);
			}
		}
		for (auto it = spawns.begin(); it != spawns.end(); ++it) {
			b2Transform xf = (*it)->GetTransform();
			b2Vec2 pos = b2Mul(xf, (*it)->GetLocalCenter());
			boxSprite.setPosition(world.GetWindowPoint(pos));
			boxSprite.setRotation(180 / b2_pi * (*it)->GetAngle());
			window.draw(boxSprite);
		}

		// Draw character
		b2Transform xf = character->GetTransform();
		b2Vec2 pos = b2Mul(xf, character->GetLocalCenter());
		characterSprite.setPosition(world.GetWindowPoint(pos));
		window.draw(characterSprite);

		// Draw bounds
		if (DEBUG)
			world.DrawBodies(window, sf::Color::Red);

		// Display window
		window.display();
	}

	return 0;
}