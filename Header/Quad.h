#pragma once

#include "QuadTreeData.h"
#include <memory>
#include <utility>
#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

enum class InsertMode
{
	Leaf = 0,
	TopLeft = 1,
	TopRight = 2,
	bottomLeft = 3,
	bottomRight = 4
};

class Quad
{
public:
	Quad(sf::Vector2u tl, sf::Vector2u br, int dim);
	~Quad();

	void Insert(sf::CircleShape* particles, int index);
	void CheckCollision(sf::CircleShape* particles, uint index, std::vector<uint>& collisionOut);
	void NewCheckCollision(sf::CircleShape* particles, std::vector<std::pair<uint, uint>>& collisions);
	void ResolveCollisions(QuadTreeData* data, std::vector<std::pair<uint, uint>>& collisions);

	void CollectQuads(std::vector<sf::Vector2u>& quadList);
	void CollectNodes(int& size, std::vector<sf::CircleShape*>& shapes); //std::vector<sf::CircleShape*> CollectNodes(Quad* quad);

private:
	void split();
	InsertMode getIntertMode(sf::Vector2f pos);
	void InternalFindCollision(sf::CircleShape* particles, std::vector<std::pair<uint, uint>>& collisions);

	sf::Vector2u m_quadWindowTL;
	sf::Vector2u m_quadWindowBR;

	std::unique_ptr<Quad> m_topLeftLeaf;
	std::unique_ptr<Quad> m_topRightLeaf;
	std::unique_ptr<Quad> m_bottomLeftLeaf;
	std::unique_ptr<Quad> m_bottomRightLeaf;

	int m_dimension = 0;

	int m_maxDimensions = 4;
	int m_maxPoints = 5;

	std::vector<uint16_t> m_indexList;
};
