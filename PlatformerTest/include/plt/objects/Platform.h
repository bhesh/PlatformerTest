/**
	@author Brian Hession

	Defines a rectangular platform
*/

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#pragma once
#endif

#ifndef PLT_OBJECTS_PLATFORM_H
#define PLT_OBJECTS_PLATFORM_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <vector>

namespace plt {
	namespace objects {

		class Platform : public sf::Drawable, public sf::Transformable {
		public:
			Platform();
			Platform(const float hx, const float hy,
				const sf::Vector2f &position = sf::Vector2f(0, 0),
				const sf::Vector2f &origin = sf::Vector2f(0, 0));
			Platform(const sf::Vector2f &size,
				const sf::Vector2f &position = sf::Vector2f(0, 0),
				const sf::Vector2f &origin = sf::Vector2f(0, 0));
			~Platform() {};
			void setSize(const float hx, const float hy);
			void setSize(const sf::Vector2f &size);
			const sf::Vector2f &getSize() const;
			sf::FloatRect getBounds() const;
			void setSpriteVector(const std::vector<std::pair<const sf::Sprite&, const sf::Transform>> *spriteVec);
			const std::vector<std::pair<const sf::Sprite&, const sf::Transform>> *getSpriteVector() const;
		private:
			virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
			sf::Vector2f m_size;
			const std::vector<std::pair<const sf::Sprite&, const sf::Transform>> *m_spriteVec;
		};
	}
}

#endif // PLT_OBJECTS_PLATFORM_H
