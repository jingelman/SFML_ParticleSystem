#include <SFML/Graphics.hpp>

#include <stdlib.h>     /* srand, rand */
#include <time.h>
//#include <math.h>       /* pow */

#include "QuadTree.h"

constexpr int C_NROFSHAPES = 512;
constexpr int C_FRAMERATE = 60;

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
	window.setFramerateLimit(C_FRAMERATE);

	sf::Clock clock;
	srand(time(NULL));

	
	sf::CircleShape shapes[C_NROFSHAPES];
	for (int i = 0; i < C_NROFSHAPES; i++)
	{
		sf::CircleShape shape(2.f);
		shape.setPosition(rand() % 500, rand() % 500);
		shapes[i] = shape;
	}

	QuadTree quadTree(window.getSize());

	for (int i = 0; i < C_NROFSHAPES; i++)
	{
		quadTree.insertNode(shapes[i]);
	}

	int s = 0;
	std::vector<sf::CircleShape*> sshapes;
	quadTree.CollectShapes(s, sshapes);

	std::vector<sf::Vector2u> lines;
	quadTree.CollectQuads(lines);


	sf::Vertex vertex;
	vertex.position = sf::Vector2f(10.f, 50.f);
	vertex.color = sf::Color::Red;
	vertex.texCoords = sf::Vector2f(100.f, 100.f);

	//sf::VertexArray line(sf::Lines, lines.size());
	//for (int i = 0; i < lines.size(); i++)
		//line[i].position = sf::Vector2f(lines[i]);
	//line[1].position = sf::Vector2f(100.f, 100.f);

	//sf::RectangleShape vetricalLine(sf::Vector2f(500.f))

	//sf::CircleShape shape(10.f);
	//shape.setFillColor(sf::Color::Green);
	//const sf::Vector2f pos = shape.getPosition();
	//shape.setPosition(pos.x + 10.0f, pos.y + 10.0f);

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		/*
		sf::Vector2f ln = sf::Vector2f(lines[3]);
		sf::RectangleShape line(ln);
		window.draw(line);
		*/
		
		for (int i = 0; i < lines.size(); i += 2)
		{
			sf::VertexArray line(sf::Lines, 8);

			sf::Vector2f p0 = sf::Vector2f(lines[i]);
			sf::Vector2f p3 = sf::Vector2f(lines[i + 1]);
			sf::Vector2f p1 = sf::Vector2f(lines[i + 1].x, lines[i].y);
			sf::Vector2f p2 = sf::Vector2f(lines[i].x, lines[i + 1].y);
			
			line[0].position = p0;
			line[1].position = p1;
			line[2].position = p0;
			line[3].position = p2;
			line[4].position = p1;
			line[5].position = p3;
			line[6].position = p2;
			line[7].position = p3;

			window.draw(line);
		}
		//window.draw(line);
		for (int i = 0; i < C_NROFSHAPES; i++)
		{
			window.draw(shapes[i]);
			
		}
		//window.draw(shapes, 25);
		//window.draw(shapes[0]);
		//window.draw(shape);
		window.display();
	}

	return 0;
}