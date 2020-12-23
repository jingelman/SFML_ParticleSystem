
#include "QuadTree.h"

#include "QuadTreeData.h"

QuadTree::QuadTree(sf::Vector2u window)
	//: m_root (size)
{
	if (m_root)
	{
		//std::cout << "ERROR";
		return;
	}

	m_root = new Quad( { 0, 0 }, window, 0);
	m_size = window;

	g_nodeData = new QuadTreeData();
}

QuadTree::QuadTree(sf::Vector2u window, int particleSize)
{
	if (m_root)
	{
		//std::cout << "ERROR";
		return;
	}

	m_root = new Quad({ 0, 0 }, window, 0);
	m_size = window;
	//m_circleShapes.reserve(particleSize);
}

QuadTree::~QuadTree() 
{ 
	if (m_root)
	{
		delete m_root;

		//m_circleShapes.clear();
		//for (auto p : m_circleShapes)
			//delete p;

		//m_currentParticleSize = 0;
	}
};

void QuadTree::insertNode(sf::CircleShape& shape) 
{ 
	if (!m_root)
	{
		//std::cout << "ERROR";
		return;
	}
		
	m_root->insert(shape);

	//m_circleShapes.push_back(shape);
	int index = g_nodeData->m_currentParticleSize;
	g_nodeData->m_circleShapes[index++] = shape;

	
	//m_circleShapes[index] = shape;
	//m_currentParticleSize++;
		
	// insert (pos and index)
}

void QuadTree::CollectQuads(std::vector<sf::Vector2u>& quadList)
{
	m_root->CollectQuads(quadList);
};

void QuadTree::CollectShapes(int& size, std::vector<sf::CircleShape*>& shapes)
{ 
	if (!m_root)
	{
		//std::cout << "ERROR";
		return;
	}

	m_root->CollectNodes(size, shapes);
};