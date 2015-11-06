#ifndef ENTITY_H
#define ENTITY_H
#include "SFML/Graphics.hpp" 
#include "Tile.h"
#include "Sprite.h"
#include "Renderer.h"


class Entity
{
protected:
	sf::Vector2f mLocation;
	sf::Vector2f mSize;
	sf::Vector2f mLastLocation;
	sf::Vector2f mVelocity;
	sf::Vector2f mAccel;
	Renderer* ren;
	Sprite* mBaseSprite;
	Sprite* mSprite;
	Sprite* mHealthSprite;
	Sprite* mHealthBarSprite;
	float mAngle = 0, mBaseAngle = 0, distance = 0;
	bool mMoving;
	bool mDrawHealth = true;
	float anmFrame = 0;
	const int mTileSize;
	float mHealth;
	int mSpriteLayer, mBaseSpriteLayer;
public:
	Entity(sf::Vector2f, int, Renderer*);
	Entity() :mTileSize(0){};
	virtual void Update(float);
	virtual void LoadAssets();
	void Collision(sf::IntRect);
	sf::Vector2f Location() const;
	sf::Vector2f Size() const;
	virtual void Draw(sf::Vector2f, float);
	sf::IntRect Rect() const;
	~Entity();
};


#endif