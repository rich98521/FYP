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
	int currentNode;
	bool mGoalReached = false;
	int mType = 0, mCore = 0, mSpawn;
	int mGoalDist;

public:
	Enemy(sf::Vector2f, int, int, Renderer*, int, int);
	void Update(float, sf::Vector2f, float);
	void Draw(sf::Vector2f, float);
	void LoadAssets();
	void SetPath(std::vector<std::pair<sf::Vector2i, float>>);
	void Shoot(sf::Vector2f, float);
	Gun* GetGun();
	bool GoalReached();
	int Type(){ return mType; }
	int Core(){ return mCore; }
	int Spawn(){ return mSpawn; }
	void SetId(int);
	sf::Vector2i GetGoal(){ return mPath.back().first; }
	void SetCore(int c)
	{
		mCore = c;
	};
	~Enemy();
};

#endif