#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"	
#include "Gun.h"	

class Enemy : public Entity
{
private:
	Gun mGun;
	std::vector<std::pair<sf::Vector2i, float>> mPath;
	std::vector<sf::Vector2f*> mPlayers;
	int currentNode;
	bool mGoalReached = false;

public:
	Enemy(sf::Vector2f, int, Renderer*, std::vector<sf::Vector2f*>);
	void Update(float, sf::Vector2f, float);
	void Draw(sf::Vector2f, float);
	void LoadAssets();
	void SetPath(std::vector<std::pair<sf::Vector2i, float>>);
	void Shoot(sf::Vector2f, float);
	Gun* GetGun();
	bool GoalReached();
	~Enemy();
};

#endif