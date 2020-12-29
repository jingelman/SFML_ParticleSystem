#include <Header/QuadTreeData.h>

#include <random>
#include <stdlib.h> /* srand, rand */
#include <time.h>

QuadTreeData* g_nodeData = nullptr;

QuadTreeData::QuadTreeData(sf::Vector2u window)
{
	m_circleShapes = new sf::CircleShape[QuadTreeHelper::g_nrOfParticles];
	m_veclocity = new sf::Vector2f[QuadTreeHelper::g_nrOfParticles];

	srand(time(NULL));
	std::random_device rd;	//Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

	for (int i = 0; i < QuadTreeHelper::g_nrOfParticles; i++)
	{
		float radius = 1.f;
		sf::CircleShape shape(radius);

		std::uniform_int_distribution<> distribX(0 + radius, window.x - radius);
		std::uniform_int_distribution<> distribY(0 + radius, window.y - radius);

		int posX = distribX(gen);
		int posY = distribY(gen);
		shape.setPosition(posX, posY);
		shape.setOrigin(radius, radius);

		m_circleShapes[i] = shape;

		sf::Vector2f vel;
		vel.x = float(rand() % 150 - 75); // * 0.1f;
		vel.y = float(rand() % 150 - 75); // * 0.1f;
		if (vel.x == 0.f)
			vel.x = 0.5f;
		if (vel.y == 0.f)
			vel.y = 0.5f;
		m_veclocity[i] = vel;

		m_currentParticleSize++;
	}
}

//QuadTreeData::QuadTreeData(int size){}

QuadTreeData::~QuadTreeData()
{
	delete[] m_circleShapes;
	delete[] m_veclocity;
}

void QuadTreeData::UpdateParticles(QuadTreeData* data, sf::Vector2u wSize, float dT)
{

	sf::Vector2f topLeft(0.f, 0.f);
	sf::Vector2f ButtomRight((float)wSize.x, (float)wSize.y);

	sf::CircleShape* shapes = data->m_circleShapes;
	sf::Vector2f* velocities = data->m_veclocity;
	const int size = data->m_currentParticleSize;

	for (int i = 0; i < size; i++)
	{
		sf::Vector2f newPos = shapes[i].getPosition();
		const float radius = shapes[i].getRadius();
		sf::Vector2f newVel = velocities[i];

		newPos.x += newVel.x * dT;
		newPos.y += newVel.y * dT;

		bool updatePos = true;

		if ((newPos.x - radius) < topLeft.x && newVel.x < 0.f)
		{
			newVel.x *= -1.f;
			updatePos = false;
		}
		else if ((newPos.y - radius) < topLeft.y && newVel.y < 0.f)
		{
			newVel.y *= -1.f;
			updatePos = false;
		}

		if ((newPos.x + radius) > ButtomRight.x && newVel.x > 0.f)
		{
			newVel.x *= -1.f;
			updatePos = false;
		}
		else if ((newPos.y + radius) > ButtomRight.y && newVel.y > 0.f)
		{
			newVel.y *= -1.f;
			updatePos = false;
		}

		//if (newVel.x == 0.0f && newVel.y == 0.0f)
		//std::cout << "Pos X: " << newPos.x << ", Pos Y: " << newPos.x << std::endl;
		//std::cout << "Vel X: " << newVel.x << ", Vel Y: " << newVel.x << std::endl;

		if (updatePos)
			shapes[i].setPosition(newPos);
		velocities[i] = newVel;
	}
}