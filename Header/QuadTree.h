#pragma once

#include <memory>
#include <vector>

#include "QuadTreeData.h"
#include <SFML/Graphics/CircleShape.hpp>

class Quad;

class QuadTree
{
public:
	QuadTree() = default;
	QuadTree(sf::Vector2u window);
	//QuadTree(sf::Vector2u window, int particleSize);
	~QuadTree();

	//void InsertNode(sf::CircleShape& shape);
	void InsertNodes(sf::CircleShape* particles, int size);
	void CheckCollisions(sf::CircleShape* particles, uint size, std::vector<uint>& collisionOut);
	void NewCheckCollision(sf::CircleShape* particles);
	void GetQuads(std::vector<sf::Vector2u>& quadList);

private:
	std::unique_ptr<Quad> m_root;
	sf::Vector2u m_size;
};
