#include "stdafx.h"
#include "Turret.h"

Turret::Turret(sf::Vector2f loc, int tSize, int t, Renderer* r, std::vector<Enemy*>* enemies)
	: Entity(loc, tSize, r), mType(t), mEnemies(enemies)
{
	mGun = Gun(r, -1, mType);
	if (mType == 0)
	{
		mGun.SetRate(.1f);
		mGun.SetColor(sf::Color(200, 200, 0, 200));
	}
}

void Turret::LoadAssets()
{
	mGun.LoadAssets();
	mSprite = new Sprite("../Sprites/Turret" + std::to_string(mType+1) + ".png");
	mBaseSprite = new Sprite("../Sprites/Turret1Base2.png");
	mScale = 1;
	mSpriteSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSize = mSpriteSize - sf::Vector2f(4, 4);
	mSprite->setOrigin(mSpriteSize.x / 2, mSpriteSize.y / 2);
	mBaseSprite->setOrigin(mSpriteSize.x / 2, mSpriteSize.y / 2);
	Entity::LoadAssets();
}

void Turret::Update(float t, sf::Vector2f offset, float scale)
{
	if (mShooting)
	{
		anmFrame += t * (mType == 0 ? 20 : 60);
		mShooting = false;
	}
}

//finds the closest enemy within range and shoots
void Turret::Shoot()
{
	float minDist = mRange;
	sf::Vector2f enemy;
	for each(Enemy* e in *mEnemies)
	{
		sf::Vector2f diff = e->Location() - mLocation;
		float dist = sqrt(diff.x*diff.x + diff.y*diff.y);
		if (dist < minDist)
		{
			minDist = dist;
			enemy = e->Location();
		}
	}
	if (minDist != mRange)
	{
		float ang = atan2(enemy.y - mLocation.y, enemy.x - mLocation.x);
		mAngle = ang / 3.14159f * 180;
		if(mGun.Shoot(mLocation, ang).size() > 0)
			SoundManager::PlaySoundEffect("TurretShoot");
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