#pragma once

#include <memory>
#include <vector>
#include "Quad.h"

#include <SFML/Graphics/CircleShape.hpp>

class Node;

class QuadTree
{
public:
	QuadTree() = default;
	QuadTree(sf::Vector2u window);
	QuadTree(sf::Vector2u window, int particleSize);
	~QuadTree();

	void insertNode(sf::CircleShape& shape);

	//void updateParticles() {  }

	void CollectQuads(std::vector<sf::Vector2u>& quadList);
	void CollectShapes(int& size, std::vector<sf::CircleShape*>& shapes);
	//std::vector<sf::CircleShape*> CollectTreeShapes(Quad* root) { if (m_root) m_root->CollectNodes(m_root); };

private:
	Quad* m_root;
	//std::unique_ptr<Quad> m_root;
	sf::Vector2u m_size;
	
	//std::vector<sf::CircleShape*> m_circleShapes;
	//sf::CircleShape m_circleShapes[PATICLES];
	//int m_currentParticleSize = 0;
};
