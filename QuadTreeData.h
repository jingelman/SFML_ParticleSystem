#pragma once

#include <SFML/Graphics/CircleShape.hpp>

constexpr int PATICLES = 64;

namespace QuadTreeHelper
{
	constexpr int g_maxDimensions = 4;
	constexpr int g_maxPoints = 5;

}

class QuadTreeData
{
public:
	QuadTreeData();


	sf::CircleShape m_circleShapes[PATICLES];
	int m_currentParticleSize = 0;

};

extern QuadTreeData* g_nodeData;