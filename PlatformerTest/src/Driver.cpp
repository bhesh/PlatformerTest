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
unsigned int WINDOW_WIDTH = 800;
unsigned int WINDOW_HEIGHT = 600;

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

	// Make the window
	plt::world::PltWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), 
		"Platformer Test", sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	window.setFont(&font); // For FPS
	window.showFps(DEBUG);

	// Make the world
	plt::world::World world(sf::Vector2f(0, 9.8f));

	// Keep track of platforms and spawns
	std::vector<b2Body *> platforms;
	std::vector<b2Body *> spawns;

	// Create platforms
	for (int i = 0; i < static_cast<int>(WINDOW_WIDTH + (31.f * SCALE));
		i += static_cast<int>(32.f * SCALE)) {
		b2Body *platform = world.CreateRect(sf::FloatRect(
			static_cast<float>(i),
			static_cast<float>(WINDOW_HEIGHT - (16.f * SCALE)),
			32.f * SCALE,
			16.f * SCALE
		));
		platforms.push_back(platform);
	}
	for (int i = WINDOW_WIDTH / 2; i < static_cast<int>(WINDOW_WIDTH + (31.f * SCALE));
		i += static_cast<int>(32.f * SCALE)) {
		b2Body *platform = world.CreateRect(sf::FloatRect(
			static_cast<float>(i),
			static_cast<float>(WINDOW_HEIGHT / 2),
			32.f * SCALE,
			16.f * SCALE
		));
		platforms.push_back(platform);
	}

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
			else if (event.type == sf::Event::MouseButtonPressed &&
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
			}
			else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Space:
					for (auto it = spawns.begin(); it != spawns.end(); ++it)
						world.DestroyBody(*it);
					spawns.clear();
					break;
				case sf::Keyboard::Tilde:
					DEBUG = !DEBUG;
					window.showFps(DEBUG);
					break;
				}
			}
		}

		// Clear the screen
		window.clear();
		window.resetView();

		// Step the world
		world.Step(dt.asSeconds());

		// Draw sprites
		for (auto it = platforms.begin(); it != platforms.end(); ++it) {
			b2Vec2 pos = (*it)->GetPosition();
			metalSprite.setPosition(world.GetWindowPoint(pos));
			window.draw(metalSprite);
		}
		for (auto it = spawns.begin(); it != spawns.end(); ++it) {
			b2Transform xf = (*it)->GetTransform();
			b2Vec2 pos = b2Mul(xf, (*it)->GetLocalCenter());
			boxSprite.setPosition(world.GetWindowPoint(pos));
			boxSprite.setRotation(180 / b2_pi * (*it)->GetAngle());
			window.draw(boxSprite);
		}

		// Draw bounds
		if (DEBUG)
			world.DrawBodies(window, sf::Color::Red);

		// Display window
		window.display();
	}

	return 0;
}