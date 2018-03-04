/**
	@author Brian Hession

	Defines the World physics
*/

#include "stdafx.h"

#include "plt/world/World.h"

#include <SFML/Graphics/CircleShape.hpp>

#include <iostream>

namespace plt {
	namespace world {

		void World::SetPixelScale(float scale) {
			m_pixelScale = scale;
		}

		float World::GetPixelScale() const {
			return m_pixelScale;
		}

		sf::Vector2f World::GetWindowPoint(b2Vec2 point) const {
			return sf::Vector2f(point.x * GetPixelScale(), point.y * GetPixelScale());
		}

		b2Vec2 World::GetWorldPoint(sf::Vector2f point) const {
			return b2Vec2(point.x / GetPixelScale(), point.y / GetPixelScale());
		}

		void World::Step(float32 seconds, int32 velIters, int32 posIters) {
			b2World::Step(seconds, velIters, posIters);
		}

		b2Body *World::CreateEdge(const sf::Vector2f &v1, const sf::Vector2f &v2,
			b2BodyType type) {

			// Make edge shape
			b2EdgeShape shape;
			shape.Set(GetWorldPoint(v1), GetWorldPoint(v2));

			// Make Fixture definitions
			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.f;
			fixtureDef.shape = &shape;

			// Make body definitions
			b2BodyDef bodyDef;
			bodyDef.type = type;

			// Construct the body with the fixture
			b2Body *body = CreateBody(&bodyDef);
			body->CreateFixture(&fixtureDef);
			return body;
		}

		b2Body *World::CreateCircle(const sf::Vector2f &pos, float radius,
			b2BodyType type) {

			// Make circle shape
			b2CircleShape shape;
			shape.m_p = GetWorldPoint(pos);
			shape.m_radius = radius / GetPixelScale();

			// Make Fixture definitions
			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.f;
			fixtureDef.shape = &shape;

			// Make body definitions
			b2BodyDef bodyDef;
			bodyDef.type = type;

			// Construct the body with the fixture
			b2Body *body = CreateBody(&bodyDef);
			body->CreateFixture(&fixtureDef);
			return body;
		}

		b2Body *World::CreateRect(const sf::FloatRect &rect, const b2BodyType type) {

			// Make polygon shape
			b2PolygonShape shape;
			shape.SetAsBox(
				rect.width / 2.f / GetPixelScale(),
				rect.height / 2.f / GetPixelScale()
			);

			// Make Fixture definitions
			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.f;
			fixtureDef.shape = &shape;

			// Make body definitions
			b2BodyDef bodyDef;
			sf::Vector2f pos;
			pos.x = rect.left + (rect.width / 2);
			pos.y = rect.top + (rect.height / 2);
			bodyDef.position = GetWorldPoint(sf::Vector2f(pos));
			bodyDef.type = type;

			// Construct the body with the fixture
			b2Body *body = CreateBody(&bodyDef);
			body->CreateFixture(&fixtureDef);
			return body;
		}

		b2Body *World::CreatePolygon(const sf::Vector2f *vertices, const int32 vertexCount,
			const b2BodyType type) {

			// Convert vertices to pixels
			b2Vec2 *vs = new b2Vec2[vertexCount];
			for (int i = 0; i < vertexCount; ++i)
				vs[i] = GetWorldPoint(vertices[i]);

			// Make polygon shape
			b2PolygonShape shape;
			shape.Set(vs, vertexCount);
			delete vs;

			// Make Fixture definitions
			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.f;
			fixtureDef.shape = &shape;

			// Make body definitions
			b2BodyDef bodyDef;
			bodyDef.type = type;

			// Construct the body with the fixture
			b2Body *body = CreateBody(&bodyDef);
			body->CreateFixture(&fixtureDef);
			return body;
		}

		void World::DrawBodies(sf::RenderTarget &target, sf::Color lineColor) const {
			for (const b2Body *b = GetBodyList(); b != 0; b = b->GetNext()) {
				b2Transform xf = b->GetTransform();
				for (const b2Fixture *bf = b->GetFixtureList(); bf != 0; bf = bf->GetNext()) {
					switch (bf->GetType()) {
					case b2Shape::e_circle:
					{
						b2CircleShape* circle = (b2CircleShape*)bf->GetShape();

						b2Vec2 center = b2Mul(xf, circle->m_p);
						float32 radius = circle->m_radius;
						b2Vec2 axis = b2Mul(xf.q, b2Vec2(1.0f, 0.0f));

						sf::CircleShape drawable(radius * GetPixelScale());
						drawable.setFillColor(sf::Color::Transparent);
						drawable.setOutlineThickness(1.f);
						drawable.setOutlineColor(lineColor);
						drawable.setOrigin(GetWindowPoint(b2Vec2(radius, radius)));
						drawable.setPosition(GetWindowPoint(center));
						target.draw(drawable);
					}
					break;

					case b2Shape::e_edge:
					{
						b2EdgeShape* edge = (b2EdgeShape*)bf->GetShape();
						b2Vec2 v1 = b2Mul(xf, edge->m_vertex1);
						b2Vec2 v2 = b2Mul(xf, edge->m_vertex2);

						sf::Vertex verts[2];
						verts[0] = sf::Vertex(GetWindowPoint(v1), lineColor);
						verts[1] = sf::Vertex(GetWindowPoint(v2), lineColor);
						target.draw(verts, 2, sf::Lines);
					}
					break;

					case b2Shape::e_chain:
					{
						b2ChainShape* chain = (b2ChainShape*)bf->GetShape();
						int32 count = chain->m_count;
						const b2Vec2* vertices = chain->m_vertices;

						b2Vec2 v1 = b2Mul(xf, vertices[0]);
						for (int32 i = 1; i < count; ++i) {
							b2Vec2 v2 = b2Mul(xf, vertices[i]);

							sf::Vertex verts[2];
							verts[0] = sf::Vertex(GetWindowPoint(v1), lineColor);
							verts[1] = sf::Vertex(GetWindowPoint(v2), lineColor);
							target.draw(verts, 2, sf::Lines);

							sf::CircleShape drawable(1.5f);
							drawable.setFillColor(sf::Color::Transparent);
							drawable.setOutlineThickness(1.f);
							drawable.setOutlineColor(lineColor);
							drawable.setOrigin(1.5f, 1.5f);
							drawable.setPosition(GetWindowPoint(v1));
							target.draw(drawable);

							v1 = v2;
						}
					}
					break;

					case b2Shape::e_polygon:
					{
						b2PolygonShape* poly = (b2PolygonShape*)bf->GetShape();
						int32 vertexCount = poly->m_count;
						b2Assert(vertexCount <= b2_maxPolygonVertices);
						sf::Vertex vertices[b2_maxPolygonVertices + 1];

						for (int32 i = 0; i < vertexCount; ++i) {
							b2Vec2 v = b2Mul(xf, poly->m_vertices[i]);
							vertices[i] = sf::Vertex(GetWindowPoint(v), lineColor);
						}
						vertices[vertexCount] = vertices[0];

						target.draw(vertices, vertexCount + 1, sf::LineStrip);
					}
					break;
					}
				}
			}
		}
	}
}