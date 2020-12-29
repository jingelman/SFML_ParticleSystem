
#include <Header/Quad.h>

#include <cmath> // std::abs

#ifdef _DEBUG
	#include <iostream>
#endif // _DEBUG

Quad::Quad(sf::Vector2u tl, sf::Vector2u br, int dim)
{
	m_quadWindowTL = tl;
	m_quadWindowBR = br;

	m_topLeftLeaf = nullptr;
	m_topRightLeaf = nullptr;
	m_bottomLeftLeaf = nullptr;
	m_bottomRightLeaf = nullptr;

	m_dimension = dim + 1;

	m_indexList.reserve(QuadTreeHelper::g_maxPoints);
}

Quad::~Quad()
{
	m_indexList.clear();
}

InsertMode Quad::getIntertMode(sf::Vector2f pos)
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

void Quad::Insert(sf::CircleShape* particles, int index)
{
	sf::CircleShape particle = particles[index];
	const sf::Vector2f& pos = particle.getPosition();
	//const float radius = particle.getRadius();

	// Outside boundaries
	//if ((pos.x - radius) < m_quadWindowTL.x || (pos.y - radius) < m_quadWindowTL.y
	//	|| (pos.x + radius) > m_quadWindowBR.x || (pos.y + radius) > m_quadWindowBR.y)
	if ((pos.x) < m_quadWindowTL.x || (pos.y) < m_quadWindowTL.y
		|| (pos.x) > m_quadWindowBR.x || (pos.y) > m_quadWindowBR.y)
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
			if (mode == InsertMode::TopLeft)
				m_topLeftLeaf->Insert(particles, index);
			else if (mode == InsertMode::TopRight)
				m_topRightLeaf->Insert(particles, index);
			else if (mode == InsertMode::bottomLeft)
				m_bottomLeftLeaf->Insert(particles, index);
			else if (mode == InsertMode::bottomRight)
				m_bottomRightLeaf->Insert(particles, index);

			return;
		}
	}

	m_indexList.emplace_back(index);

	// split and reinsert points
	if (m_indexList.size() > QuadTreeHelper::g_maxPoints && m_dimension < QuadTreeHelper::g_maxDimensions)
	{
		split();

		while (!m_indexList.empty())
		{
			int i = m_indexList.back();
			sf::CircleShape shape = particles[i];
			m_indexList.pop_back();
			InsertMode mode = getIntertMode(shape.getPosition());

			if (mode == InsertMode::TopLeft)
				m_topLeftLeaf->Insert(particles, i);
			else if (mode == InsertMode::TopRight)
				m_topRightLeaf->Insert(particles, i);
			else if (mode == InsertMode::bottomLeft)
				m_bottomLeftLeaf->Insert(particles, i);
			else if (mode == InsertMode::bottomRight)
				m_bottomRightLeaf->Insert(particles, i);
		}
	}
}

void Quad::CheckCollision(sf::CircleShape* particles, uint index, std::vector<uint>& collisionOut)
{
	const sf::Vector2f& pos = particles[index].getPosition();

	if (m_topLeftLeaf)
	{
		InsertMode mode = getIntertMode(pos);

		if (mode != InsertMode::Leaf)
		{
			if (mode == InsertMode::TopLeft)
				m_topLeftLeaf->CheckCollision(particles, index, collisionOut);
			else if (mode == InsertMode::TopRight)
				m_topRightLeaf->CheckCollision(particles, index, collisionOut);
			else if (mode == InsertMode::bottomLeft)
				m_bottomLeftLeaf->CheckCollision(particles, index, collisionOut);
			else if (mode == InsertMode::bottomRight)
				m_bottomRightLeaf->CheckCollision(particles, index, collisionOut);

			return;
		}
	}

	for (const uint i : m_indexList)
	{
		const sf::Vector2f& coll = particles[i].getPosition();
		const float radius = particles[i].getRadius();
		const float dist = powf(pos.x - coll.x, 2) + powf(pos.y - coll.y, 2);
		if (i != index && dist <= powf(radius * 2.f, 2.f))
		{
			collisionOut.emplace_back(index);
			collisionOut.emplace_back(i);
			particles[index].setFillColor(sf::Color::Red);
			particles[i].setFillColor(sf::Color::Red);
			return; // support only collisions with 2 particles for now
		}
	}
}

void Quad::NewCheckCollision(sf::CircleShape* particles, std::vector<std::pair<uint, uint>>& collisions)
{
	if (m_topLeftLeaf)
		m_topLeftLeaf->NewCheckCollision(particles, collisions);
	if (m_topRightLeaf)
		m_topRightLeaf->NewCheckCollision(particles, collisions);
	if (m_bottomLeftLeaf)
		m_bottomLeftLeaf->NewCheckCollision(particles, collisions);
	if (m_bottomRightLeaf)
		m_bottomRightLeaf->NewCheckCollision(particles, collisions);

	if (!m_indexList.empty())
		InternalFindCollision(particles, collisions);
}

void Quad::InternalFindCollision(sf::CircleShape* particles, std::vector<std::pair<uint, uint>>& collisions)
{
	std::vector<bool> dirtyList(QuadTreeHelper::g_maxPoints, false);
	UNUSED(collisions);
	for (uint i = 0; i < m_indexList.size(); i++) //for (const uint i : m_indexList)
	{
		bool isDirty1 = dirtyList[i];
		if (isDirty1)
			continue;

		const uint index1 = m_indexList[i];
		const sf::Vector2f& p1Pos = particles[index1].getPosition();
		for (uint j = i + 1; j < m_indexList.size(); j++) //for (const uint j : m_indexList)
		{
			bool isDirty2 = dirtyList[j];
			if (isDirty2)
				continue;

			const uint index2 = m_indexList[j];
			if (index1 == index2)
				continue;

			const sf::Vector2f& p2Pos = particles[index2].getPosition();
			const float radius = particles[index1].getRadius();
			const float dist = powf(p1Pos.x - p2Pos.x, 2) + powf(p1Pos.y - p2Pos.y, 2);
			if (dist <= powf(radius * 2.f, 2.f))
			{

				dirtyList[i] = true;
				dirtyList[j] = true;
				collisions.emplace_back(std::make_pair(index1, index2));
#ifdef _DEBUG
				particles[index1].setFillColor(sf::Color::Red);
				particles[index2].setFillColor(sf::Color::Red);
#endif
				return; // support only collisions with 2 particles for now
			}
		}
	}
}

void Quad::ResolveCollisions(QuadTreeData* data, std::vector<std::pair<uint, uint>>& collisions)
{
	sf::CircleShape* particles = data->m_circleShapes;
	sf::Vector2f* veclocities = data->m_veclocity;
	/*
	for (auto c : collisions)
	{
		uint& index1 = c.first;
		uint& index2 = c.second;

		sf::CircleShape& p1 = particles[index1];
		sf::CircleShape& p2 = particles[index2];
		const sf::Vector2f& p1Pos = p1.getPosition();
		const sf::Vector2f& p2Pos = p2.getPosition();
		sf::Vector2f& p1Vel = veclocities[index1];
		sf::Vector2f& p2Vel = veclocities[index2];
		if (p1Vel.x > 0.f && p2Vel.x > 0.f)
		{
			if (p1Pos.x > p2Pos.x)
			{
				p2Vel.x *= -1.f;
			}
			else
			{
				p1Vel.x *= -1.f;
			}
		}
		else
		{
			p1Vel.x *= -1.f;
			p2Vel.x *= -1.f;
		}

		if (p1Vel.y > 0.f && p2Vel.y > 0.f)
		{

			if (p1Pos.y > p2Pos.y)
			{
				p2Vel.y *= -1.f;
			}
			else
			{
				p1Vel.y *= -1.f;
			}
		}
		else
		{
			p1Vel.y *= -1.f;
			p2Vel.y *= -1.f;
		}
	}
	*/
	for (auto c : collisions)
	{
		uint& index1 = c.first;
		uint& index2 = c.second;

		const sf::CircleShape& p1 = particles[index1];
		const sf::CircleShape& p2 = particles[index2];
		const sf::Vector2f& p1Pos = p1.getPosition();
		const sf::Vector2f& p2Pos = p2.getPosition();
		sf::Vector2f& p1Vel = veclocities[index1];
		sf::Vector2f& p2Vel = veclocities[index2];

		// Normal
		//var collisionNormal = Position - otherBall.Position;
		//collisionNormal.Normalize();
		//collisionNormal
		sf::Vector2f collisionNormal = p1Pos - p2Pos;
		collisionNormal /= sqrtf(collisionNormal.x * collisionNormal.x + collisionNormal.y * collisionNormal.y);

		// ja = (-1(1 + Cr)* (Va - Vb)* n)/(1 /ma + 1/mb ) = {m = 1, Cr = 1}
		// j =  -1*(2)*(Va - Vb)* n / 2
		sf::Vector2f j = -1.f * (p1Vel - p2Vel);
		float ja = j.x * collisionNormal.x + j.y * collisionNormal.y;

		// New vel
		// Va = Va + (Ja/m)*n
		// Vb = Vb - (Ja/m)*n
		sf::Vector2f k = ja * collisionNormal;

		p1Vel += k;
		p2Vel -= k;
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
	sf::Vector2u p1((m_quadWindowTL.x + m_quadWindowBR.x) / 2, m_quadWindowTL.y);
	sf::Vector2u p2(m_quadWindowTL.x, (m_quadWindowTL.y + m_quadWindowBR.y) / 2);
	sf::Vector2u p3((m_quadWindowTL.x + m_quadWindowBR.x) / 2, (m_quadWindowTL.y + m_quadWindowBR.y) / 2);
	sf::Vector2u p4(m_quadWindowBR.x, (m_quadWindowTL.y + m_quadWindowBR.y) / 2);
	sf::Vector2u p5((m_quadWindowTL.x + m_quadWindowBR.x) / 2, m_quadWindowBR.y);
	sf::Vector2u p6 = m_quadWindowBR;

	m_topLeftLeaf = std::make_unique<Quad>(p0, p3, m_dimension);
	m_topRightLeaf = std::make_unique<Quad>(p1, p4, m_dimension);
	m_bottomLeftLeaf = std::make_unique<Quad>(p2, p5, m_dimension);
	m_bottomRightLeaf = std::make_unique<Quad>(p3, p6, m_dimension);

	m_dimension++;
}

void Quad::CollectQuads(std::vector<sf::Vector2u>& quadList)
{
	if (!m_topLeftLeaf)
	{
		quadList.push_back(m_quadWindowTL);
		quadList.push_back(m_quadWindowBR);
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
}

void Quad::CollectNodes(int& size, std::vector<sf::CircleShape*>& shapes) //std::vector<sf::CircleShape*> Quad::CollectNodes(Quad* quad)
{
	//std::vector<sf::CircleShape*> shapes;
	//int count = 0;

	if (!m_topLeftLeaf)
	{
		//size += m_Nodes.size();
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