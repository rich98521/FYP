#include "stdafx.h"
#include "Explosion.h"

Explosion::Explosion(sf::Vector2f loc, int tSize, Renderer* r, float scale)
	: Entity(loc, tSize, r) 
{
	mCollision = false;
	mScale = scale;
}

void Explosion::LoadAssets()
{
	mSprite = new Sprite("../Sprites/Explosion.png");
	mSize = sf::Vector2f(56, mSprite->getTextureRect().height);
	mSpriteSize = sf::Vector2f(56, mSprite->getTextureRect().height);
	mSprite->setOrigin(mSize.x / 2, mSize.y / 2 + 25);
	ren->Add(mSprite, mSpriteLayer);
}

//animates the core
void Explosion::Update(float t, sf::Vector2f offset, float scale)
{
	anmFrame += t * 60;
}

void Explosion::Draw(sf::Vector2f offset, float scale)
{
	int baseMax = 26;
	if (anmFrame <= 26)
	{
		mSprite->setTextureRect(sf::IntRect(56 * ((int)anmFrame % baseMax), 0, 56, mSpriteSize.y));
		mSprite->setScale(scale * mScale * mHScale, scale * mScale * mHScale);
		mSprite->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset))* scale);
	}
	else
		mAlive = false;
}