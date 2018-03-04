/**
	@author Brian Hession

	Defines the World physics
*/

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#pragma once
#endif

#ifndef PLT_WINDOW_WORLD_H
#define PLT_WINDOW_WORLD_H

#include <SFML/System/Vector2.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <Box2D/Box2D.h>

namespace plt {
	namespace world {

		class World : public b2World {
		public:
			World(const sf::Vector2f &gravity) :
				b2World(b2Vec2(gravity.x, gravity.y)),
				m_pixelScale(30.f) {};
			World(const World &world) :
				b2World(world),
				m_pixelScale(world.GetPixelScale()) {};
			~World() {};
			void SetPixelScale(float scale);
			float GetPixelScale() const;
			sf::Vector2f GetWindowPoint(b2Vec2 point) const;
			b2Vec2 GetWorldPoint(sf::Vector2f point) const;
			void Step(float32 seconds, int32 velIters = 8, int32 posIters = 3);
			b2Body *CreateEdge(const sf::Vector2f &v1, const sf::Vector2f &v2, const b2BodyType type = b2_staticBody);
			b2Body *CreateCircle(const sf::Vector2f &pos, float radius, const b2BodyType type = b2_staticBody);
			b2Body *CreateRect(const sf::FloatRect &rect, const b2BodyType type = b2_staticBody);
			b2Body *CreatePolygon(const sf::Vector2f *vertices, const int32 vertexCount, const b2BodyType type = b2_staticBody);
			void DrawBodies(sf::RenderTarget &target, sf::Color lineColor = sf::Color::Red) const;
			//b2Body *CreateLiquid();
		private:
			float m_pixelScale;
		};
	}
}

#endif //PLT_WINDOW_WORLD_H