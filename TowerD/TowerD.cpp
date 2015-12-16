#include "stdafx.h"
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#define _USE_MATH_DEFINES
#include <math.h>
#include "Level.h"
#include "Renderer.h"
#include "Menu.h"
#include <list>

int main()
{
	Config::Init();
	sf::RenderWindow window(sf::VideoMode(Config::ScreenWidth(), Config::ScreenHeight(), 32), "TowerDefense");
	srand(time(NULL));
	const int tileSize = 32;
	Renderer r = Renderer();
	Level level(tileSize, sf::Vector2i(Config::ScreenWidth(), Config::ScreenHeight()), &r);
	Menu menu(&r, &window, &level);
	sf::Clock clock;
	sf::Vector2i mPos;
	while (window.isOpen())
	{
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
				window.close();
			if (Event.type == sf::Event::KeyPressed || Event.type == sf::Event::KeyReleased || Event.type == sf::Event::MouseButtonPressed || Event.type == sf::Event::MouseButtonReleased || Event.type == sf::Event::MouseMoved)
			{
				menu.ProcessInput(Event);
				if (!menu.GamePaused())
					level.ProcessInput(Event);
			}
			if (Event.type == sf::Event::MouseMoved)
				mPos = sf::Vector2i(Event.mouseMove.x, Event.mouseMove.y);
		}

		window.clear();
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();

		menu.Update();
		if (!menu.GamePaused()){
			level.Update(time, mPos);
			level.CheckCollision();
			level.Draw(&window);
		}
		r.Draw(&window);

		window.display();
	}

	return EXIT_SUCCESS;
}
