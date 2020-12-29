#pragma once

#include <SFML/Graphics/CircleShape.hpp>

#define UNUSED(x) (void)(x)

namespace QuadTreeHelper
{
constexpr int g_maxDimensions = 10;
constexpr int g_maxPoints = 8;
constexpr int g_nrOfParticles = 2056;
}

class QuadTreeData
{
public:
	QuadTreeData(sf::Vector2u window);
	//QuadTreeData(int size);
	~QuadTreeData();

	void UpdateParticles(QuadTreeData* data, sf::Vector2u size, float dT);

	sf::CircleShape* m_circleShapes;
	sf::Vector2f* m_veclocity;
	int m_currentParticleSize = 0;
};

extern QuadTreeData* g_nodeData;