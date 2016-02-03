#include "stdafx.h"
#include "Missile.h"

Missile::Missile(sf::Vector2f loc, int tSize, Renderer* r, sf::Vector2f target)
	: Entity(loc, tSize, r), mTarget(target)
{
	//mLocation += sf::Vector2f(tSize - 8, tSize - 8);
	mCollision = false;
	mHeight = 48;
	mStartHeight = mHeight;
	sf::Vector2f diff = mTarget - mLocation;
	mStartDist = sqrt(diff.x * diff.x + diff.y * diff.y);
	mAngle = atan2(diff.y, diff.x);
	mAccel = sf::Vector2f(cos(mAngle), sin(mAngle)) * 800.f;
	canMove = true;
	mCollision = false;
	mHealth = 1;	
}

void Missile::LoadAssets()
{
	mSprite = new Sprite("../Sprites/Missile.png");
	mSize = sf::Vector2f(mSprite->getTextureRect().width, mSprite->getTextureRect().height);
	mSpriteSize = sf::Vector2f(mSprite->getTextureRect().width, mSprite->getTextureRect().height);
	mSprite->setOrigin(mSize.x / 2, mSize.y / 2);
	ren->Add(mSprite, mBaseSpriteLayer);
}

bool Missile::ReachedGoal()
{
	return mDist < 5;
}

//animates the core
void Missile::Update(float t, sf::Vector2f offset, float scale)
{
	Entity::Update(t, offset, scale);
	mAngle = mBaseAngle;
	mDist = sqrt((mLocation.x - mTarget.x)*(mLocation.x - mTarget.x) + (mLocation.y - mTarget.y)*(mLocation.y - mTarget.y));
	mHeight = (mDist / mStartDist)*mHeight;
}

void Missile::Draw(sf::Vector2f offset, float scale)
{
	mSprite->setScale(scale * mScale * mHScale, scale * mScale * mHScale);
	mSprite->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset))* scale);
	mSprite->setRotation(mAngle);
}