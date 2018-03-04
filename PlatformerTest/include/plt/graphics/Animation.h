/**
	@author Brian Hession

	Animated sprite
*/

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#pragma once
#endif

#ifndef PLT_GRAPHICS_ANIMATION_H
#define PLT_GRAPHICS_ANIMATION_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <vector>

namespace plt {
	namespace graphics {

		class Animation {
		public:
			Animation() : m_texture(NULL) {};
			void addFrame(sf::IntRect rect);
			void setSpriteSheet(const sf::Texture& texture);
			const sf::Texture* getSpriteSheet() const;
			std::size_t getSize() const;
			const sf::IntRect& getFrame(std::size_t n) const;
		private:
			std::vector<sf::IntRect> m_frames;
			const sf::Texture* m_texture;
		};
	}
}

#endif // PLT_GRAPHICS_ANIMATION_H