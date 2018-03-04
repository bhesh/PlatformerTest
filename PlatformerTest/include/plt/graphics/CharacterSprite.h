/**
	@author Brian Hession

	Extends AnimatedSprite to hold many different Animations.

	A unique name is given to these animations for readability.
*/

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#pragma once
#endif

#ifndef PLT_GRAPHICS_CHARACTERSPRITE_H
#define PLT_GRAPHICS_CHARACTERSPRITE_H

#include <SFML/System/String.hpp>

#include <SFML/Graphics/Texture.hpp>

#include <map>

#include "plt/graphics/Animation.h"
#include "plt/graphics/AnimatedSprite.h"

namespace plt {
	namespace graphics {
	
		class CharacterSprite : public AnimatedSprite {
		public:
			CharacterSprite() : AnimatedSprite(sf::seconds(1.f), false, true) {};
			~CharacterSprite();
			sf::Texture* newTexture(const sf::String &key);
			sf::Texture* getTexture(const sf::String &key);
			void destroyTexture(const sf::String &key);
			Animation* newAnimation(const sf::String &key);
			Animation* getAnimation(const sf::String &key);
			void destroyAnimation(const sf::String &key);
			virtual void setAnimation(const sf::String &key);
			const Animation* getCurrentAnimation() const;
		private:
			std::map<sf::String, sf::Texture*> m_textures;
			std::map<sf::String, Animation*> m_animations;
		};
	}
}

#endif // PLT_GRAPHICS_CHARACTERSPRITE_H