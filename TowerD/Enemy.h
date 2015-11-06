#ifndef ENEMY_H
#define ENEMY_H
#include "SFML/Graphics.hpp" 
#include "Entity.h"	
#include "Gun.h"	

class Enemy : public Entity
{
private:
	Gun mGun;

public:
	Enemy(sf::Vector2f, int, Renderer*);
	void Update(float, sf::Vector2f);
	void LoadAssets();
};

#endif