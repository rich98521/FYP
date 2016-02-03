#ifndef LEVEL_H
#define LEVEL_H
using namespace std;
#include<iostream>
#include<fstream>
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "Core.h"
#include "Renderer.h"
#include "Enemy.h"
#include "Turret.h"
#include <queue>
#include <list>
#include "SoundManager.h"
#include "Explosion.h"
#include "Missile.h"

struct Node
{
	Node(sf::Vector2i pos)
		:Position(pos), Marked(false)
	{
	}; 
	Node(){};
	Node* Previous;
	int Distance;
	bool Marked, Visited;
	sf::Vector2i Position;
};

struct Compare {
	bool operator() (Node* first, Node* second) const
	{
		return (first->Distance > second->Distance);
	}
};

class Level
{
private:
	Renderer* mRen;
	Camera mCam;
	int mTileSize;
	sf::Vector2i mScreenSize, mPlayerSpawn, mMousePos;
	std::vector<sf::Vector2i> mDirections;
	std::vector<sf::Vector2i> mEnemySpawns;
	std::vector<Player*> mPlayers;
	std::vector<Enemy*> mEnemies;
	std::vector<Core*> mCores;
	std::vector<Entity*> mEntities;
	std::vector<std::vector<Tile*>> mTiles;
	std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int>>>>> mWaves;
	std::vector<std::vector<std::pair<sf::Vector2i, float>>> mArrowPaths;
	std::vector<std::vector<std::pair<sf::Vector2i, float>>> mWeakPath;
	std::vector<std::vector<Sprite*>> mArrows;
	std::vector<sf::Vector2f*> mPlayerLocs;
	std::vector<Turret*> mTurrets;
	std::vector<Explosion*> mExplosions;
	std::vector<Missile*> mMissiles;
	std::vector<Tile*> mWalls;
	sf::Vector2i mMapSize;
	Node** mNodes;
	bool CalcArrowPath(sf::Vector2i);
	template<typename T>
	void ClearVector(std::vector<T>*);
	void AddEntity(Entity*);
	void RemoveEntity(Entity*);
	bool mDefensePhase = true;
	bool mWin = false;
	sf::Clock mGameClock;
	sf::Clock mRespawnTimer;
	bool mPlayerSpawning = false;
	int mWave = 0;
	int mEnemiesLeft = 0;
	int mNextSpawnTime = 0;
	std::vector<std::pair<sf::Vector2i, float>> CalcPath(sf::Vector2i, sf::Vector2i, std::vector<std::vector<Tile*>>*);
	void SetTile(Tile*, int, int);
	std::vector<std::pair<sf::Vector2i, float>> CalcWeakPoint(int);
	bool mGameOver;
	int mCurrentLevel;

public:
	Level(int, sf::Vector2i, Renderer*);
	void LoadLevel(int);
	void Update(float);
	void Draw(sf::RenderWindow*);
	void CheckCollision();
	void ProcessInput(sf::Event);
	Player* GetPlayer();
	std::vector<Turret*>* GetTurrets() { return &mTurrets; };
	Camera* GetCam(){ return &mCam; };
	void RestartCurrentWave();
	bool GameOver(){ return mGameOver; };
	bool Won(){ return mWin; };
	void LoadNextLevel(){ LoadLevel(mCurrentLevel + 1); };
	int GetWave(){ return mWave; }

};

#endif