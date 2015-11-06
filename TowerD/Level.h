#ifndef LEVEL_H
#define LEVEL_H
using namespace std;
#include<iostream>
#include<fstream>
#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "Core.h"
#include "Renderer.h"
#include "Enemy.h"
#include <queue>
#include <list>

struct Node
{
	Node(sf::Vector2i pos)
		:Position(pos), Marked(false)
	{
	};
	Node(){};
	Node* Previous;
	int Distance;
	bool Marked;
	sf::Vector2i Position;
};

struct Compare {
	bool operator() (Node* first, Node* second) const
	{
		return (first->Distance < second->Distance);
	}
};

class Level
{
private:
	Renderer* mRen;
	Camera mCam;
	int mTileSize;
	sf::Vector2i mScreenSize;
	sf::Vector2i mPlayerSpawn;
	std::vector<sf::Vector2i> mDirections;
	std::vector<sf::Vector2i> mEnemySpawns;
	std::vector<Player*> mPlayers;
	std::vector<Core*> mCores;
	std::vector<std::vector<Tile*>> mTiles;
	std::vector<std::map<int, std::pair<int, int>>> mWaves;
	std::vector<std::vector<std::pair<sf::Vector2i, float>>> mArrowPaths;
	void CalcArrowPath();
	sf::Vector2i mMapSize;
	Node** mNodes;
	Enemy* enemy;
public:
	Level(int, sf::Vector2i, Renderer*);
	void LoadLevel(int);
	void Update(float, sf::Vector2i);
	void Draw(sf::RenderWindow*);
	void CheckCollision();
	void ProcessInput(sf::Event);

};

#endif