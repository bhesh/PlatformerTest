/**
	@author Brian Hession

	Animated sprite
*/

#include "stdafx.h"

#include "plt/graphics/CharacterSprite.h"

namespace plt {
	namespace graphics {
	
		CharacterSprite::~CharacterSprite() {
			for (auto it = m_animations.begin(); it != m_animations.end(); ++it)
				delete it->second;
			m_animations.clear();
			for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
				delete it->second;
			m_textures.clear();
		}

		sf::Texture* CharacterSprite::newTexture(const sf::String &key) {
			if (m_textures.find(key) != m_textures.end())
				throw std::logic_error("Key alread exists");
			sf::Texture *texture = new sf::Texture();
			m_textures.insert(std::pair<sf::String, sf::Texture*>(key, texture));
			return texture;
		}

		sf::Texture* CharacterSprite::getTexture(const sf::String &key) {
			if (m_textures.find(key) == m_textures.end())
				throw std::logic_error("Key not found");
			return m_textures.at(key);
		}

		void CharacterSprite::destroyTexture(const sf::String &key) {
			sf::Texture *texture = getTexture(key);
			m_textures.erase(key);
			delete texture;
		}

		Animation* CharacterSprite::newAnimation(const sf::String &key) {
			if (m_animations.find(key) != m_animations.end())
				throw std::logic_error("Key alread exists");
			Animation *animation = new Animation();
			m_animations.insert(std::pair<sf::String, Animation*>(key, animation));
			return animation;
		}

		Animation* CharacterSprite::getAnimation(const sf::String &key) {
			if (m_animations.find(key) == m_animations.end())
				throw std::logic_error("Key not found");
			return m_animations.at(key);
		}

		void CharacterSprite::destroyAnimation(const sf::String &key) {
			Animation *animation = getAnimation(key);
			m_animations.erase(key);
			delete animation;
		}

		void CharacterSprite::setAnimation(const sf::String &key) {
			AnimatedSprite::setAnimation(*getAnimation(key));
		}

		const Animation* CharacterSprite::getCurrentAnimation() const {
			return AnimatedSprite::getAnimation();
		}
	}
}