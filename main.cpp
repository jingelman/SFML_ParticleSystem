#include <Header/QuadTree.h>
#include <Header/QuadTreeData.h>

#include <SFML/Graphics.hpp>

#include <string> // std::string, std::to_string

#define LINESOLUTION 1

#define UNUSED(x) (void)(x)

constexpr uint C_FRAMERATE = 1000;

#ifdef _DEBUG
void DrawDebugQuads(sf::RenderWindow& window, QuadTree& qTree);
#endif

#include <math.h> /* sin */
#define PI 3.14159265

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Particle System");
	window.setFramerateLimit(C_FRAMERATE);
	window.setVerticalSyncEnabled(false);

	g_nodeData = new QuadTreeData(window.getSize());
	/*
	float r = 200.f;
	float x = window.getSize().x / 2;
	float y = window.getSize().y / 2;

	float cos0r = r * cosf(0);
	UNUSED(cos0r);
	float cospi4r = r * cosf(PI / 4);
	UNUSED(cospi4r);
	float cospi2r = r * cosf(PI / 2);
	UNUSED(cospi2r);
	float cos3pi4r = r * cosf(3 * PI / 4);
	UNUSED(cos3pi4r);
	float cospir = r * cosf(PI);
	UNUSED(cospir);
	float cos5pi4r = r * cosf(5 * PI / 4);
	UNUSED(cos5pi4r);
	float cos3pi2r = r * cosf(3 * PI / 2);
	UNUSED(cos3pi2r);
	float cos7pi4r = r * cosf(7 * PI / 4);
	UNUSED(cos7pi4r);

	float sin0r = r * sinf(0);
	UNUSED(sin0r);
	float sinpi4r = r * sinf(PI / 4);
	UNUSED(sinpi4r);
	float sinpi2r = r * sinf(PI / 2);
	UNUSED(sinpi2r);
	float sin3pi4r = r * sinf(3 * PI / 4);
	UNUSED(sin3pi4r);
	float sinpir = r * sinf(PI);
	UNUSED(sinpir);
	float sin5pi4r = r * sinf(5 * PI / 4);
	UNUSED(sin5pi4r);
	float sin3pi2r = r * sinf(3 * PI / 2);
	UNUSED(sin3pi2r);
	float sin7pi4r = r * sinf(7 * PI / 4);
	UNUSED(sin7pi4r);

	sf::VertexArray triangleFan(sf::TriangleFan, 8);
	triangleFan[0].position = sf::Vector2f(x, y);
	triangleFan[0].color = sf::Color::Blue;
	triangleFan[1].position = sf::Vector2f(x - cos0r, y - sin0r);
	triangleFan[1].color = sf::Color::Blue;
	triangleFan[2].position = sf::Vector2f(x - cospi4r, y - sinpi4r);
	triangleFan[2].color = sf::Color::Blue;
	triangleFan[3].position = sf::Vector2f(x - cospi2r, y - sinpi2r);
	triangleFan[3].color = sf::Color::Blue;
	triangleFan[4].position = sf::Vector2f(x - cos3pi4r, y - sin3pi4r);
	triangleFan[4].color = sf::Color::Blue;
	triangleFan[5].position = sf::Vector2f(x - cospir, y - sinpir);
	triangleFan[5].color = sf::Color::Blue;
	triangleFan[6].position = sf::Vector2f(x - cos5pi4r, y - sin5pi4r);
	triangleFan[6].color = sf::Color::Blue;
	triangleFan[7].position = sf::Vector2f(x - cos3pi2r, y - sin3pi2r);
	triangleFan[7].color = sf::Color::Blue;
	triangleFan[8].position = sf::Vector2f(x - cos7pi4r, y - sin7pi4r);
	triangleFan[8].color = sf::Color::Blue;
*/
	sf::Clock clock;

	sf::Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
	{
		std::cout << "Error: Coulnt load font";
	}
	sf::Text ms;
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
			}
		}
		//#ifdef _DEBUG
		sf::Time elapsed = clock.restart();
		std::string elapsedMs = std::to_string(elapsed.asMilliseconds());
		ms.setString("ms: " + elapsedMs);
		window.draw(ms);
		//#endif

		//window.draw(triangleFan);
		window.display();
		//############ END ############
	}

	return 0;
}

#ifdef _DEBUG
void DrawDebugQuads(sf::RenderWindow& window, QuadTree& quadTree)
{
	std::vector<sf::Vector2u> quadPoints;
	quadTree.GetQuads(quadPoints);

	#if LINESOLUTION == 0
	sf::VertexArray lines(sf::Lines, 8);
	#elif LINESOLUTION == 1
	sf::VertexArray lines(sf::Lines, quadPoints.size() * 8 / 2);
	size_t lineIndex = 0;
	#endif

	for (size_t i = 0; i < quadPoints.size(); i += 2)
	{
		// p0-------p1
		//  |		|
		// p2-------p3
		sf::Vector2f p0 = sf::Vector2f(quadPoints[i]);
		sf::Vector2f p1 = sf::Vector2f(quadPoints[i + 1].x, quadPoints[i].y);
		sf::Vector2f p2 = sf::Vector2f(quadPoints[i].x, quadPoints[i + 1].y);
		sf::Vector2f p3 = sf::Vector2f(quadPoints[i + 1]);

	#if LINESOLUTION == 0
		lines[0].position = p0;
		lines[1].position = p1;
		lines[2].position = p0;
		lines[3].position = p2;
		lines[4].position = p1;
		lines[5].position = p3;
		lines[6].position = p2;
		lines[7].position = p3;

		window.draw(lines);
	#else
		lines[lineIndex].position = p0;
		lines[lineIndex + 1].position = p1;
		lines[lineIndex + 2].position = p0;
		lines[lineIndex + 3].position = p2;
		lines[lineIndex + 4].position = p1;
		lines[lineIndex + 5].position = p3;
		lines[lineIndex + 6].position = p2;
		lines[lineIndex + 7].position = p3;
		lineIndex += 8;
	#endif
	}
	#if LINESOLUTION == 1
	window.draw(lines);
	#endif
}
#endif
