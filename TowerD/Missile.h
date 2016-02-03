#ifndef MISSILE_H
#define MISSILE_H
#include "Entity.h"	
#include "SoundManager.h"

class Missile : public Entity
{
private:
	sf::Vector2f mTarget;
	float mStartDist, mStartHeight, mDist;
public:
	Missile(sf::Vector2f, int, Renderer*, sf::Vector2f);
	void Update(float, sf::Vector2f, float);
	void LoadAssets();
	void Draw(sf::Vector2f, float);
	bool ReachedGoal();
};


#endif