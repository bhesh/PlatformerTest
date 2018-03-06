/**
	@author Brian Hession

	Defines a rectangular platform
*/

#include "plt/objects/Platform.h"

namespace plt {
	namespace objects {

		// Public

		Platform::Platform() {
			Platform::Platform(32.f, 32.f);
		}

		Platform::Platform(const float hx, const float hy,
			const sf::Vector2f &position, const sf::Vector2f &origin) {
			Platform::Platform(sf::Vector2f(hx, hy), position, origin);
		}

		Platform::Platform(const sf::Vector2f &size,
			const sf::Vector2f &position, const sf::Vector2f &origin) {
			m_size = size;
			setPosition(position);
			setOrigin(origin);
			m_spriteVec = NULL;
		}

		void Platform::setSize(const float hx, const float hy) {
			m_size = sf::Vector2f(hx, hy);
		}

		void Platform::setSize(const sf::Vector2f &size) {
			m_size = size;
		}

		const sf::Vector2f &Platform::getSize() const {
			return m_size;
		}

		sf::FloatRect Platform::getBounds() const {
			sf::Vector2f lowerBounds = getPosition() - getOrigin();
			return sf::FloatRect(lowerBounds.x, lowerBounds.y,
				lowerBounds.x + m_size.x, lowerBounds.y + m_size.y);
		}

		void Platform::setSpriteVector(const std::vector<std::pair<const sf::Sprite&, const sf::Transform>> *spriteVec) {
			m_spriteVec = spriteVec;
		}

		const std::vector<std::pair<const sf::Sprite&, const sf::Transform>> *Platform::getSpriteVector() const {
			return m_spriteVec;
		}

		// Private

		void Platform::draw(sf::RenderTarget &target, sf::RenderStates states) const {
			states.transform *= getTransform();
			for (auto pair = m_spriteVec->begin(); pair != m_spriteVec->end(); ++pair) {
				const sf::Sprite &sprite = pair->first;
				sf::Transform xf = pair->second;
				sf::RenderStates spriteStates = states;
				spriteStates.transform *= xf;
				target.draw(sprite, spriteStates);
			}
		}
	}
}