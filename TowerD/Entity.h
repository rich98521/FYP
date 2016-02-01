#ifndef ENTITY_H
#define ENTITY_H
#include "Tile.h"
#include "Sprite.h"
#include "Renderer.h"


class Entity
{
protected:
	sf::Vector2f mLocation;
	sf::Vector2f mSize;
	sf::Vector2f mSpriteSize;
	sf::Vector2f mLastLocation;
	sf::Vector2f mVelocity;
	sf::Vector2f mAccel;
	Renderer* ren;
	Sprite* mBaseSprite;
	Sprite* mSprite;
	Sprite* mHealthSprite;
	Sprite* mHealthBarSprite;
	const float pi = 3.1415926535f;
	const float toDeg = 1 / 180 * pi;
	const float toRad = 1 / pi * 180;
	float mAngle = 0, mBaseAngle = 0, distance = 0;
	bool mMoving;
	bool mDrawHealth = true;
	float anmFrame = 0;
	const int mTileSize;
	float mHealth;
	int mSpriteLayer, mBaseSpriteLayer;
	float mSpeed = 1;
	float mScale = 1;
	bool mAlive = true;
	float mHScale = 1;
	float mHeight = 0;
	bool mOnWall = false, canMove = false;

public:
	Entity(sf::Vector2f, int, Renderer*);
	Entity() :mTileSize(0){};
	virtual void Update(float, sf::Vector2f, float);
	virtual void LoadAssets();
	void Collision(sf::IntRect);
	void Collision(sf::Vector2f, float);
	sf::Vector2f Location() const;
	sf::Vector2f Size() const;
	virtual void Draw(sf::Vector2f, float);
	sf::IntRect Rect() const;
	virtual bool Hit(float);
	float Health();
	float Height();
	void SetOnWall(bool);
	~Entity();
};


#endif