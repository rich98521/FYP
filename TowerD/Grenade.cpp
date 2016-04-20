#include "stdafx.h"
#include "Grenade.h"

Grenade::Grenade(sf::Vector2f loc, int tSize, Renderer* r, sf::Vector2f vel, float height)
	: Entity(loc, tSize, r)
{
	mCollision = true;
	mHeight = height + 10;
	mVelocity = vel;
	canMove = true;
	mHealth = 0.001f;
	mScale = .4f;
	mClock.restart();
	mRotation = (0.3f + (rand() % 100) / 40.f) * (1 - (rand() % 2) * 2);
	mCoRestitution = 0.65f;
}

void Grenade::LoadAssets()
{
	mSprite = new Sprite("../Sprites/Grenade.png");
	mSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSpriteSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSprite->setOrigin(mSize.x / 2, mSize.y / 2);
	ren->Add(mSprite, mBaseSpriteLayer);
}

void Grenade::Update(float t, sf::Vector2f offset, float scale)
{
	Entity::Update(t, offset, scale);
	mRotation *= 0.998f;
	mAngle += mRotation;
	if (mClock.getElapsedTime().asSeconds() > 3)
		Explode();
	float change = -t * 220;
	mHeight += change;
	if (mOnWall)
		mHeight = fmax(mHeight, mTileSize);
	mHeight = fmax(mHeight, 0);
	mFrame += 0.1f;
}

void Grenade::Explode()
{
	Entity::Hit(200);
	mAlive = false;
}

void Grenade::Draw(sf::Vector2f offset, float scale)
{
	int baseMax = (int)(mSprite->getTexture()->getSize().x) / (int)mSpriteSize.y;
	mSprite->setTextureRect(sf::IntRect(mSpriteSize.y * ((int)mFrame % baseMax), 0, mSpriteSize.y, mSpriteSize.y));
	mSprite->setScale(scale * mScale * mHScale, scale * mScale * mHScale);
	mSprite->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset))* scale);
	mSprite->setRotation(mAngle);
}