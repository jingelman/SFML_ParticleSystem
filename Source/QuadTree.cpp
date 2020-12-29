
#include <Header/Quad.h>
#include <Header/QuadTree.h>
#include <Header/QuadTreeData.h>

//#include <thread>

class pair;

QuadTree::QuadTree(sf::Vector2u window)
//: m_root (size)
{

	if (m_root)
	{
		//std::cout << "ERROR";
		return;
	}
	sf::Vector2u s(0, 0);
	m_root = std::make_unique<Quad>(s, window, 0);
	m_size = window;
}
/*
QuadTree::QuadTree(sf::Vector2u window, int particleSize)
{
#ifdef _DEBUG
	std::cout << particleSize;
#endif

	if (m_root)
	{
		//std::cout << "ERROR";
		return;
	}
	sf::Vector2u s(0, 0);
	m_root = std::make_unique<Quad>(s, window, 0);
	m_size = window;
}
*/
QuadTree::~QuadTree()
{
	/*
	if (m_root)
	{
		delete m_root;
		m_root = nullptr;
	}
	*/
	//m_root = nullptr;
}
/*
void QuadTree::InsertNode(sf::CircleShape& shape)
{
	if (!m_root)
	{
		//std::cout << "ERROR";
		return;
	}

	//m_root->Insert(shape);

	// insert (pos and index)
}
*/
void QuadTree::InsertNodes(sf::CircleShape* particles, int size)
{
	if (!m_root)
	{
		//std::cout << "ERROR";
		return;
	}

	for (int i = 0; i < size; i++)
	{
		//m_root->Insert(particles[i]);
		m_root->Insert(particles, i);
	}

	// insert (pos and index)
}

void QuadTree::CheckCollisions(sf::CircleShape* particles, uint size, std::vector<uint>& collisionOut)
{
	// TODO
	// Dont iteratie through the list
	// Find alla leafs and iterate thiugh particle list in leaf
	for (uint i = 0; i < size; i++)
	{
		//m_root->Insert(particles[i]);
		//m_root->Insert(particles, i);
		m_root->CheckCollision(particles, i, collisionOut);
	}
}

void QuadTree::NewCheckCollision(sf::CircleShape* particles)
{
	std::vector<std::pair<uint, uint>> collisions;
	m_root->NewCheckCollision(particles, collisions);

	if (!collisions.empty())
		m_root->ResolveCollisions(g_nodeData, collisions);
}

void QuadTree::GetQuads(std::vector<sf::Vector2u>& quadList)
{
	m_root->CollectQuads(quadList);
}

/*
void QuadTree::GetShapes(int& size, std::vector<sf::CircleShape*>& shapes)
{
	if (!m_root)
	{
		//std::cout << "ERROR";
		return;
	}

	m_root->CollectNodes(size, shapes);
}
*/