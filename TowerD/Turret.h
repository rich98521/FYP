#ifndef TURRET_H
#define TURRET_H
#include "Enemy.h"	
#include "Gun.h"	

class Turret : public Entity
{
private:
	Gun mGun;
	std::vector<std::pair<sf::Vector2i, float>> mPath;
	std::vector<Enemy*>* mEnemies;
	bool mShooting = false;
	float mRange = 64;

public:
	Turret(sf::Vector2f, int, Renderer*, std::vector<Enemy*>*);
	void Draw(sf::Vector2f, float);
	void Update(float, sf::Vector2f, float);
	void LoadAssets();
	void Shoot();
	Gun* GetGun();
};


#endif