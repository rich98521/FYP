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

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "Core.h"
#include "Renderer.h"
#include "Level.h"
#include <list>
#include <string>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "TowerDefense");
	srand(time(NULL));
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");
	sf::Text text;
	text.setFont(font);
	text.setPosition(5, 5);
	text.setCharacterSize(12);
	const int tileSize = 32;
	Renderer r = Renderer();
	Level level(tileSize, sf::Vector2i(800, 600), &r);
	//sf::Vector2i toCheck[8] = { sf::Vector2i(1, 0), sf::Vector2i(-1, 0), sf::Vector2i(0, 1), sf::Vector2i(0, -1), sf::Vector2i(1, 1), sf::Vector2i(-1, -1), sf::Vector2i(-1, 1), sf::Vector2i(1, -1) };
	//std::vector<Tile> tiles[20];
	//for (int x = 0; x < 20; x++){
	//	for (int y = 0; y < 20; y++)
	//	{
	//		if (y == 0 || y == 19 || x == 0 || x == 19 || (x == 10 && y == 10) || (x == 10 && y == 11) || (x == 8 && y == 10) || (x == 8 && y == 11))
	//			tiles[x].push_back(Tile(sf::Vector2i(x * tileSize, y * tileSize), tileSize, 2, &r));
	//		else
	//			tiles[x].push_back(Tile(sf::Vector2i(x * tileSize, y * tileSize), tileSize, 0, &r));
	//	}
	//}
	//for (int x = 0; x < 20; x++)
	//	for (int y = 0; y < 20; y++)
	//		tiles[x][y].LoadAssets();

	window.setMouseCursorVisible(false);

	//Player p(sf::Vector2f(64, 64), tileSize, &r);
	//p.LoadAssets();
	//Camera cam(p.AimPos(), p.Pos());
	//Core c(sf::Vector2f(256, 256), tileSize, &r);
	//c.LoadAssets();
	sf::Clock clock;	
	sf::Vector2i mPos;
	while (window.isOpen())
	{
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
				window.close();

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();
			if (Event.type == sf::Event::KeyPressed || Event.type == sf::Event::KeyReleased || Event.type == sf::Event::MouseButtonPressed || Event.type == sf::Event::MouseButtonReleased || Event.type == sf::Event::MouseMoved)
			{
				//p.ProcessInput(Event, cam.Offset(), cam.Scale());
				level.ProcessInput(Event);
			}
			if (Event.type == sf::Event::MouseMoved)
				mPos = sf::Vector2i(Event.mouseMove.x, Event.mouseMove.y);
		}

		window.clear();
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		//p.Update(time, cam.Offset(), cam.Scale());
		//c.Update(time);
		//cam.Update(mPos);
		level.Update(time, mPos);
		//sf::Vector2i index = sf::Vector2i((p.Location().x + tileSize/2) / tileSize, (p.Location().y + tileSize / 2) / tileSize);
		//for each (sf::Vector2i t in toCheck)
		//{
		//	if ((p.Location().x - (tileSize + p.Size().x) / 2 < (index.x + t.x) * tileSize && 
		//		 p.Location().x + (tileSize + p.Size().x) / 2 > (index.x + t.x) * tileSize) && 
		//		(p.Location().y - (tileSize + p.Size().y) / 2 < (index.y + t.y) * tileSize &&
		//		 p.Location().y + (tileSize + p.Size().y) / 2 > (index.y + t.y) * tileSize))
		//		if (tiles[index.x + t.x][index.y + t.y].Type() == 2)
		//			p.Collision(tiles[index.x + t.x][index.y + t.y].Rect());
		//}
		level.CheckCollision();
		//for (int x = 0; x < 20; x++)
		//	for (int y = 0; y < 20; y++)
		//		tiles[x][y].Draw(cam.Offset(), cam.Scale());
		//p.Draw(cam.Offset(), cam.Scale());
		//c.Draw(cam.Offset(), cam.Scale());

		level.Draw(&window);
		r.Draw(&window);

		//text.setString(std::to_string(p.Location().x) + ", " + std::to_string(p.Location().y) + "\n" + std::to_string(index.x) + ", " + std::to_string(index.y) + "\n" + std::to_string(1/time));
		//window.draw(text);

		window.display();
	}

	return EXIT_SUCCESS;
}
