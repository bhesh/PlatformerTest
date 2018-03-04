/**
	@author Brian Hession

	Animated sprite
*/

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#pragma once
#endif

#ifndef PLT_GRAPHICS_ANIMATEDSPRITE_H
#define PLT_GRAPHICS_ANIMATEDSPRITE_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include "plt/graphics/Animation.h"

namespace plt {
	namespace graphics {
	
		class AnimatedSprite : public sf::Drawable, public sf::Transformable {
		public:
			explicit AnimatedSprite(sf::Time frameTime = sf::seconds(0.1f),
				bool paused = false, bool looped = true) :
				m_animation(NULL), m_frameTime(frameTime), m_currentFrame(0),
				m_isPaused(paused), m_isLooped(looped) {};
			void update(sf::Time deltaTime);
			virtual void setAnimation(const Animation& animation);
			void setFrameTime(sf::Time time);
			void play();
			void play(const Animation& animation);
			void pause();
			void stop();
			void setLooped(bool looped);
			void setColor(const sf::Color& color);
			const Animation* getAnimation() const;
			sf::FloatRect getLocalBounds() const;
			sf::FloatRect getGlobalBounds() const;
			bool isLooped() const;
			bool isPlaying() const;
			sf::Time getFrameTime() const;
			void setFrame(std::size_t newFrame, bool resetTime = true);
		private:
			const Animation* m_animation;
			sf::Time m_frameTime;
			sf::Time m_currentTime;
			std::size_t m_currentFrame;
			bool m_isPaused;
			bool m_isLooped;
			const sf::Texture* m_texture;
			sf::Vertex m_vertices[4];
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		};
	}
}

#endif // PLT_GRAPHICS_ANIMATEDSPRITE_H