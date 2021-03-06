#include "stdafx.h"
#include "Core.h"

Core::Core(sf::Vector2f loc, int tSize, Renderer* r)
	: Entity(loc, tSize, r)
{
	mCollision = false;
	mHealth = 500;
}

void Core::LoadAssets()
{
	mSprite = new Sprite("../Sprites/Core.png");
	mBaseSprite = new Sprite("../Sprites/CoreBase.png");
	mSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSpriteSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSprite->setOrigin(mSize.x / 2, mSize.y / 2);
	mBaseSprite->setOrigin(mSize.x / 2, mSize.y / 2);
	Entity::LoadAssets();
}

//animates the core
void Core::Update(float t, sf::Vector2f offset, float scale)
{
	anmFrame += t * 10;
	mAngle += t * 20;
}	