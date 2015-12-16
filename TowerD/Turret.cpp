#include "stdafx.h"
#include "Turret.h"

Turret::Turret(sf::Vector2f loc, int tSize, Renderer* r, std::vector<Enemy*>* enemies)
	: Entity(loc, tSize, r), mGun(r, -1), mEnemies(enemies)
{
	mGun.SetRate(.1f);
}

void Turret::LoadAssets()
{
	mSprite = new Sprite("../Sprites/Turret1.png");
	mBaseSprite = new Sprite("../Sprites/Turret1Base.png");
	mScale = 1;
	mSpriteSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSize = mSpriteSize;
	mSprite->setOrigin(mSpriteSize.x / 2, mSpriteSize.y / 2);
	mBaseSprite->setOrigin(mSpriteSize.x / 2, mSpriteSize.y / 2);
	Entity::LoadAssets();
}

void Turret::Update(float t, sf::Vector2f offset, float scale)
{
	if (mShooting)
	{
		anmFrame += t * 20;
		mShooting = false;
	}
}

//finds the closest enemy if any within range and shoots
void Turret::Shoot()
{
	float minDist = mRange;
	sf::Vector2f enemy;
	for each(Enemy* e in *mEnemies)
	{
		sf::Vector2f diff = e->Location() - mLocation;
		float dist = sqrt(diff.x*diff.x + diff.y*diff.y);
		if (dist < mRange && dist < minDist)
		{
			minDist = dist;
			enemy = e->Location();
		}
	}
	if (minDist != mRange)
	{
		float ang = atan2(enemy.y - mLocation.y, enemy.x - mLocation.x);
		mAngle = ang / 3.14159f * 180;
		mGun.Shoot(mLocation, ang);
		mShooting = true;
	}
}

void Turret::Draw(sf::Vector2f offset, float scale)
{
	mGun.Draw(mLocation, offset, scale);
	Entity::Draw(offset, scale);
}

Gun* Turret::GetGun()
{
	return &mGun;
}#include "stdafx.h"
#include "Turret.h"

Turret::Turret(sf::Vector2f loc, int tSize, Renderer* r, std::vector<Enemy*>* enemies)
: Entity(loc, tSize, r), mGun(r, -1), mEnemies(enemies)
{
	mGun.SetRate(.1f);
}

void Turret::LoadAssets()
{
	mSprite = new Sprite("../Sprites/Turret1.png");
	mBaseSprite = new Sprite("../Sprites/Turret1Base.png");
	mScale = 1;
	mSpriteSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSize = mSpriteSize;
	mSprite->setOrigin(mSpriteSize.x / 2, mSpriteSize.y / 2);
	mBaseSprite->setOrigin(mSpriteSize.x / 2, mSpriteSize.y / 2);
	Entity::LoadAssets();
}

void Turret::Update(float t, sf::Vector2f offset, float scale)
{
	if (mShooting)
	{
		anmFrame += t * 20;
		mShooting = false;
	}
}

//finds the closest enemy if any within range and shoots
void Turret::Shoot()
{
	float minDist = mRange;
	sf::Vector2f enemy;
	for each(Enemy* e in *mEnemies)
	{
		sf::Vector2f diff = e->Location() - mLocation;
		float dist = sqrt(diff.x*diff.x + diff.y*diff.y);
		if (dist < mRange && dist < minDist)
		{
			minDist = dist;
			enemy = e->Location();
		}
	}
	if (minDist != mRange)
	{
		float ang = atan2(enemy.y - mLocation.y, enemy.x - mLocation.x);
		mAngle = ang / 3.14159f * 180;
		mGun.Shoot(mLocation, ang);
		mShooting = true;
	}
}

void Turret::Draw(sf::Vector2f offset, float scale)
{
	mGun.Draw(mLocation, offset, scale);
	Entity::Draw(offset, scale);
}

Gun* Turret::GetGun()
{
	return &mGun;
}