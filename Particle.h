#pragma once

#include <SFML/System/Vector2.hpp>


class Particles
{
public:
	Particles(sf::CircleShape shape/*, const float r*/)
	{
		//m_posX = pos.x;
		//m_posY = pos.y;
		//m_radii = r;

		m_shape = shape;
	};

	void update()

	//sf::Vector2f getPos() { return sf::Vector2f(m_posX, m_posY); }
	sf::CircleShape getShape() { return m_shape; }


private:
	//float m_posX;
	//float m_posY;
	//float m_radii;
	sf::Vector2f velocity;

	sf::CircleShape m_shape;
};