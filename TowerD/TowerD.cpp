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
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#define _USE_MATH_DEFINES
#include <math.h>
#include "Level.h"
#include "Renderer.h"
#include "Menu.h"
#include "SoundManager.h"
#include "Network.h"
#include "curl/curl.h"
#include <list>

int main()
{
	Config::Init();
	curl_global_init(CURL_GLOBAL_ALL);
	Network::Init();
	sf::RenderWindow window(sf::VideoMode(Config::ScreenWidth(), Config::ScreenHeight(), 32), "TowerDefense");
	srand(time(NULL));
	const int tileSize = 32;
	Renderer r = Renderer();
	SoundManager snd;
	snd.Init();
	Level level(tileSize, sf::Vector2i(Config::ScreenWidth(), Config::ScreenHeight()), &r);
	Menu menu(&r, &window, &level, level.GetCam());
	sf::Clock clock;
	float drawInterval = 0.0166f, updateInterval = 0.0043f;
	float drawTime = 0, updateTime = 0;
	bool ctrl = false, shft = false , alt = false;
	unsigned int modifierKeys = 0;
	
	while (window.isOpen())
	{
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::KeyPressed)
			{
				if (Event.key.code == sf::Keyboard::LControl || Event.key.code == sf::Keyboard::RControl)
					ctrl = true;
				else if (Event.key.code == sf::Keyboard::LShift || Event.key.code == sf::Keyboard::RShift)
					shft = true;
				else if (Event.key.code == sf::Keyboard::LAlt || Event.key.code == sf::Keyboard::RAlt)
					alt = true;
			}
			else if (Event.type == sf::Event::KeyReleased)
			{
				if (Event.key.code == sf::Keyboard::LControl || Event.key.code == sf::Keyboard::RControl)
					ctrl = false;
				else if (Event.key.code == sf::Keyboard::LShift || Event.key.code == sf::Keyboard::RShift)
					shft = false;
				else if (Event.key.code == sf::Keyboard::LAlt || Event.key.code == sf::Keyboard::RAlt)
					alt = false;
			}
			modifierKeys = (ctrl ? ModifierKeys::Control : 0) | (shft ? ModifierKeys::Shift : 0) | (alt ? ModifierKeys::Alt : 0);
			if (Event.type == sf::Event::Closed)
				window.close();
			else if (Event.type == sf::Event::KeyPressed)
			{
				menu.ProcessInput(Event, modifierKeys);
				if (!menu.GamePaused())
					level.ProcessInput(Event, modifierKeys);
			}
			else if (Event.type == sf::Event::MouseButtonPressed || Event.type == sf::Event::JoystickButtonPressed || Event.type == sf::Event::JoystickMoved || Event.type == sf::Event::MouseMoved)
			{
				if (!menu.ProcessInput(Event, modifierKeys))
					level.ProcessInput(Event, modifierKeys);
			}
			else if (Event.type == sf::Event::KeyReleased || Event.type == sf::Event::MouseButtonReleased || Event.type == sf::Event::JoystickButtonReleased)
			{
				menu.ProcessInput(Event, modifierKeys);
				level.ProcessInput(Event, modifierKeys);
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
