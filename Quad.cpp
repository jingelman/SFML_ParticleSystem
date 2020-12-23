#pragma once

#include "Quad.h"

#include <iostream>


class Node
{
public:
	Node(sf::CircleShape shape/*, const float r*/)
	{
		//m_posX = pos.x;
		//m_posY = pos.y;
		//m_radii = r;

		m_shape = shape;
	};

	//sf::Vector2f getPos() { return sf::Vector2f(m_posX, m_posY); }
	sf::CircleShape getShape() { return m_shape; }
	

private:
	//float m_posX;
	//float m_posY;
	//float m_radii;

	sf::CircleShape m_shape;
};

Quad::Quad(sf::Vector2u tl, sf::Vector2u br, int dim)
{
	m_quadWindowTL = tl;
	m_quadWindowBR = br;

	m_topLeftLeaf		 = nullptr;
	m_topRightLeaf		 = nullptr;
	m_bottomLeftLeaf	 = nullptr;
	m_bottomRightLeaf	 = nullptr;

	m_dimension = dim + 1;
};

Quad::~Quad()
{
	delete m_topLeftLeaf;
	delete m_topRightLeaf;
	delete m_bottomLeftLeaf;
	delete m_bottomRightLeaf;
};

const InsertMode Quad::getIntertMode(sf::Vector2f pos)
{
	InsertMode mode = InsertMode::Leaf;

	sf::Vector2f origo = sf::Vector2f((m_quadWindowTL.x + m_quadWindowBR.x) / 2, (m_quadWindowTL.y + m_quadWindowBR.y) / 2);
	const bool topQuads = pos.y <= origo.y;
	const bool leftQuads = pos.x <= origo.x;

	if (topQuads)
	{
		if (leftQuads) // TopLeftLeaf quad
			mode = InsertMode::TopLeft;
		else
			mode = InsertMode::TopRight;
	}
	else
	{
		if (leftQuads) // Top right quad
			mode = InsertMode::bottomLeft;
		else // BottomRightLeaf quad
			mode = InsertMode::bottomRight;
	}

	return mode;
}

void Quad::insert(sf::CircleShape shape/*, const float radius*/)
{
	sf::Vector2f pos = shape.getPosition();

	// Outside boundaries
	if (pos.x < m_quadWindowTL.x || pos.y < m_quadWindowTL.y || pos.x > m_quadWindowBR.x || pos.y > m_quadWindowBR.y)
	{
#ifdef _DEBUG
		std::cout << "Outside" << std::endl;
#endif // _DEBUG
		return;
	}

	if (m_topLeftLeaf)
	{
		InsertMode mode = getIntertMode(pos);

		if (mode != InsertMode::Leaf)
		{
			//insert(pos/*, radius*/);

			if (mode == InsertMode::TopLeft)
				m_topLeftLeaf->insert(shape);
			else if (mode == InsertMode::TopRight)
				m_topRightLeaf->insert(shape);
			else if (mode == InsertMode::bottomLeft)
				m_bottomLeftLeaf->insert(shape);
			else if (mode == InsertMode::bottomRight)
				m_bottomRightLeaf->insert(shape);

			return;
		}
	}

	m_Nodes.emplace_back(new Node(shape/*, radius*/));

	// max level and max points
	/*
	if (m_Nodes.size() < m_maxPoints ) // && m_dimension < m_maxDimensions
	{
		m_Nodes.emplace_back(new Node(pos, radius));

#ifdef _DEBUG
		std::cout << "Point:" << " X = " << pos.x << ", Y = " << pos.y << std::endl;
#endif
		return;
	}
	*/


	// split and reinsert points
	if (m_Nodes.size() > QuadTreeHelper::g_maxPoints && m_dimension < QuadTreeHelper::g_maxDimensions)
	{
		split();

		auto itEnd = m_Nodes.crend();

		while (!m_Nodes.empty())
		{
			//sf::Vector2f pos = (*it)->getPos();
			sf::CircleShape shape = m_Nodes.back()->getShape();
			//sf::Vector2f pos = m_Nodes.back()->getPos();
			m_Nodes.pop_back();
			InsertMode mode = getIntertMode(shape.getPosition());

			if (mode == InsertMode::TopLeft)
				m_topLeftLeaf->insert(shape);
			else if (mode == InsertMode::TopRight)
				m_topRightLeaf->insert(shape);
			else if (mode == InsertMode::bottomLeft)
				m_bottomLeftLeaf->insert(shape);
			else if (mode == InsertMode::bottomRight)
				m_bottomRightLeaf->insert(shape);

			
		}
		
	}
}


void Quad::split()
{
	// Quads
	// p0-------p1-------x
	//  |		|		|
	// p2-------p3-------p4
	//  |		|		|
	//  x-------p5-------p6

	sf::Vector2u p0 = m_quadWindowTL;
	sf::Vector2u p1( (m_quadWindowTL.x + m_quadWindowBR.x) / 2, m_quadWindowTL.y );
	sf::Vector2u p2( m_quadWindowTL.x, (m_quadWindowTL.y + m_quadWindowBR.y) / 2 );
	sf::Vector2u p3( (m_quadWindowTL.x + m_quadWindowBR.x) / 2, (m_quadWindowTL.y + m_quadWindowBR.y) / 2 );
	sf::Vector2u p4( m_quadWindowBR.x, (m_quadWindowTL.y + m_quadWindowBR.y) / 2);
	sf::Vector2u p5( (m_quadWindowTL.x + m_quadWindowBR.x) / 2, m_quadWindowBR.y);
	sf::Vector2u p6 = m_quadWindowBR;

	m_topLeftLeaf =		new Quad(p0, p3, m_dimension);
	m_topRightLeaf =	new Quad(p1, p4, m_dimension);
	m_bottomLeftLeaf =	new Quad(p2, p5, m_dimension);
	m_bottomRightLeaf = new Quad(p3, p6, m_dimension);
	
	m_dimension++;
}

void Quad::CollectQuads(std::vector<sf::Vector2u>& quadList)
{

	if (!m_topLeftLeaf)
	{
		if (!m_Nodes.empty())
		{
			quadList.push_back(m_quadWindowTL);
			quadList.push_back(m_quadWindowBR);
		}
		//return;
	}
	else
	{
		if (m_topLeftLeaf)
			m_topLeftLeaf->CollectQuads(quadList);

		if (m_topRightLeaf)
			m_topRightLeaf->CollectQuads(quadList);

		if (m_bottomLeftLeaf)
			m_bottomLeftLeaf->CollectQuads(quadList);

		if (m_bottomRightLeaf)
			m_bottomRightLeaf->CollectQuads(quadList);
	}
	
	
	
};

void Quad::CollectNodes(int& size, std::vector<sf::CircleShape*>& shapes) //std::vector<sf::CircleShape*> Quad::CollectNodes(Quad* quad) 
{
	//std::vector<sf::CircleShape*> shapes;
	//int count = 0;

	if (!m_topLeftLeaf)
	{
		size += m_Nodes.size();
		//for (auto node : m_Nodes)
			//shapes.push_back(&node->getShape());//shapes.push_back(&node->getShape());
		//return count;
	}
	else
	{
		if (m_topLeftLeaf)
			m_topLeftLeaf->CollectNodes(size, shapes);

		if (m_topRightLeaf)
			m_topRightLeaf->CollectNodes(size, shapes);

		if (m_bottomLeftLeaf)
			m_bottomLeftLeaf->CollectNodes(size, shapes);

		if (m_bottomRightLeaf)
			m_bottomRightLeaf->CollectNodes(size, shapes);
	}
		
	


	//return shapes;
}