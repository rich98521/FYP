#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"	
#include "Gun.h"	
#include "SoundManager.h"

class Enemy : public Entity
{
private:
	Gun mGun;
	std::vector<std::pair<sf::Vector2i, float>> mPath;
	std::vector<sf::Vector2f*> mPlayers;
	int currentNode;
	bool mGoalReached = false;
	int mType = 0, mCore = 0;
	int mGoalDist;

public:
	Enemy(sf::Vector2f, int, int, Renderer*, std::vector<sf::Vector2f*>, int);
	void Update(float, sf::Vector2f, float);
	void Draw(sf::Vector2f, float);
	void LoadAssets();
	void SetPath(std::vector<std::pair<sf::Vector2i, float>>);
	void Shoot(sf::Vector2f, float);
	Gun* GetGun();
	bool GoalReached();
	sf::Vector2i GetGoal(){ return mPath.back().first; }
	int GetCore()
	{
		return mCore;
	};
	void SetCore(int c)
	{
		mCore = c;
	};
	int Type(){ return mType; }
	~Enemy();
};

#endif