#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "Entity.h"	
#include "SoundManager.h"

class Explosion : public Entity
{
private:
public:
	Explosion(sf::Vector2f, int, Renderer*, float);
	void Update(float, sf::Vector2f, float);
	void LoadAssets();
	void Draw(sf::Vector2f, float);
	float GetScale(){ return mScale; }
};


#endif