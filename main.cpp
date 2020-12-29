
#include <Header/QuadTree.h>
#include <Header/QuadTreeData.h>

#include <SFML/Graphics.hpp>

#include <string> // std::string, std::to_string

constexpr uint C_FRAMERATE = 60;

#ifdef _DEBUG
void DrawDebugQuads(sf::RenderWindow& window, QuadTree& qTree);
#endif

int main()
{
	sf::RenderWindow window(sf::VideoMode(400, 400), "SFML works!");
	window.setFramerateLimit(C_FRAMERATE);

	g_nodeData = new QuadTreeData(window.getSize());

	sf::Clock clock;

	sf::Text ms;
	sf::Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
	{
		std::cout << "Error: Coulnt load font";
	}
	ms.setFont(font);
	ms.setCharacterSize(24); // in pixels, not points!
	ms.setFillColor(sf::Color::Red);
	ms.setStyle(sf::Text::Bold | sf::Text::Underlined);

	while (window.isOpen())
	{
		//########### EVENT ############
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//############ END ############

		//######## SIMULATION #########
		//float t = clock.getElapsedTime().asSeconds();
		//std::cout << "Time1: " << t << std::endl;
		g_nodeData->UpdateParticles(g_nodeData, window.getSize(), 1.f / 60.f);

		QuadTree quadTree(window.getSize());

		quadTree.InsertNodes(g_nodeData->m_circleShapes, g_nodeData->m_currentParticleSize);

		quadTree.NewCheckCollision(g_nodeData->m_circleShapes);

		//std::vector<uint> collisionOut;
		//quadTree.CheckCollisions(g_nodeData->m_circleShapes, g_nodeData->m_currentParticleSize, collisionOut);
		//sort(collisionOut.begin(), collisionOut.end());
		//collisionOut.erase(unique(collisionOut.begin(), collisionOut.end()), collisionOut.end());
		//############ END ############

		//########## RENDER ###########
		window.clear();

#ifdef _DEBUG
		DrawDebugQuads(window, quadTree);
#endif

		{ // Draw particles
			int size = g_nodeData->m_currentParticleSize;
			sf::CircleShape* s = g_nodeData->m_circleShapes;
			for (int i = 0; i < size; i++)
			{
				window.draw(s[i]);
				s[i].setFillColor(sf::Color::White);
			} /*
			for (const uint c : collisionOut)
			{
				sf::CircleShape temp = s[c];
				temp.setFillColor(sf::Color(255, 0, 0));
				window.draw(temp);
			}*/
		}
		//#ifdef _DEBUG
		sf::Time elapsed = clock.restart();
		std::string elapsedMs = std::to_string(elapsed.asMilliseconds());
		ms.setString("ms: " + elapsedMs);
		window.draw(ms);
		//#endif
		window.display();
		//############ END ############
	}

	return 0;
}

#ifdef _DEBUG
void DrawDebugQuads(sf::RenderWindow& window, QuadTree& quadTree)
{
	std::vector<sf::Vector2u> lines;
	quadTree.GetQuads(lines);
	for (size_t i = 0; i < lines.size(); i += 2)
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
}
#endif