#pragma once

#include <memory>
#include <vector>
#include "QuadTreeData.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>


class Node;

enum class InsertMode
{
	Leaf		= 0,
	TopLeft		= 1,
	TopRight	= 2,
	bottomLeft	= 3,
	bottomRight = 4
};

class Quad
{
public:
	Quad(sf::Vector2u tl, sf::Vector2u br, int dim);
	~Quad();

	const InsertMode getIntertMode(sf::Vector2f pos);
	void insert(sf::CircleShape shape);
	
	const Node& SearchForNode(std::unique_ptr<Node> node);
	void CollectQuads(std::vector<sf::Vector2u>& quadList);
	void CollectNodes(int& size, std::vector<sf::CircleShape*>& shapes);//std::vector<sf::CircleShape*> CollectNodes(Quad* quad);

private:
	void split();


	sf::Vector2u m_quadWindowTL;
	sf::Vector2u m_quadWindowBR;

	Quad* m_topLeftLeaf;
	Quad* m_topRightLeaf;
	Quad* m_bottomLeftLeaf;
	Quad* m_bottomRightLeaf;

	int m_dimension = 0;

	int m_maxDimensions = 4;
	int m_maxPoints = 5;

	//std::vector<std::unique_ptr<Node>> m_Nodes;
	std::vector<Node*> m_Nodes;
};
