#ifndef GRENADE_H
#define GRENADE_H
#include "Entity.h"	

class Grenade : public Entity
{
private:
	sf::Clock mClock;
	float mFrame, mRotation;
public:
	Grenade(sf::Vector2f, int, Renderer*, sf::Vector2f, float);
	void Update(float, sf::Vector2f, float);
	void LoadAssets();
	void Draw(sf::Vector2f, float);
	void Explode();
};


#endif