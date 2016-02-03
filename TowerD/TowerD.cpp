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
#include "SoundManager.h"
#include <list>

int main()
{
	Config::Init();
	sf::RenderWindow window(sf::VideoMode(Config::ScreenWidth(), Config::ScreenHeight(), 32), "TowerDefense");
	srand(time(NULL));
	const int tileSize = 32;
	Renderer r = Renderer();
	SoundManager snd;
	snd.Init();
	Level level(tileSize, sf::Vector2i(Config::ScreenWidth(), Config::ScreenHeight()), &r);
	Menu menu(&r, &window, &level, level.GetCam());
	sf::Clock clock;
	float drawInterval = 0.0166f, updateInterval = 0.0033f;
	float drawTime = 0, updateTime = 0;
	
	while (window.isOpen())
	{
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
				window.close();
			else if (Event.type == sf::Event::KeyPressed)
			{
				menu.ProcessInput(Event);
				if (!menu.GamePaused())
					level.ProcessInput(Event);
			}
			else if (Event.type == sf::Event::MouseButtonPressed|| Event.type == sf::Event::MouseMoved)
			{
				if (!menu.ProcessInput(Event))
					level.ProcessInput(Event);
			}
			else if (Event.type == sf::Event::KeyReleased || Event.type == sf::Event::MouseButtonReleased)
			{
				menu.ProcessInput(Event);
				level.ProcessInput(Event);
			}
		}
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		drawTime += time;
		updateTime += time;

		menu.Update();
		if (!menu.GamePaused())
		{
			if (updateTime > updateInterval)
			{
				level.Update(time);
				level.CheckCollision();
				updateTime = 0;
			}
		}

		if (drawTime > drawInterval)
		{
			window.clear();

			level.Draw(&window);
			r.Draw(&window);

			window.display();
			drawTime -= drawInterval;
		}
	}

	return EXIT_SUCCESS;
}
