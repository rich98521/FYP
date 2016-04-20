#include "stdafx.h"
#include "Turret.h"

Turret::Turret(sf::Vector2f loc, int tSize, int t, Renderer* r, std::vector<Enemy*>* enemies)
	: Entity(loc, tSize, r), mType(t), mEnemies(enemies),
	mUpgradeCosts{ { 15, 30, 45, 75, 100 } }
{
	mUpgradeTable[0] = { { .05f, .005f, 5 } };
	mUpgradeTable[1] = { { .1f, .01f, 10 } };
	mUpgradeTable[2] = { { .2f, .015f, 15 } };
	mUpgradeTable[3] = { { .3f, .025, 23 } };
	mUpgradeTable[4] = { { .5f, .04, 33 } };
	mGun = Gun(r, -1, mType);
	mBaseRange = 64;
	if (mType == 0)
	{
		mBaseRate = .4f;
		mBaseDamage = 0.3f;
		mGun.SetRate(mBaseRate);
		mGun.SetColor(sf::Color(200, 200, 0, 200));
		mGun.SetDamage(mBaseDamage);
	}
	else if (mType == 1)
	{
		mBaseRate = 3.f;
		mBaseDamage = 2.f;
		mGun.SetRate(mBaseRate);
		mGun.SetDamage(mBaseDamage);
	}
	else if (mType == 2)
	{
		mBaseRate = 0.24f;
		mBaseDamage = 0.1f;
		mGun.SetDamage(mBaseDamage);
	}
	mCost = 15;
}

void Turret::SetId(int i)
{
	Entity::SetId(i);
	mGun.SetOwnerId(i);
}

void Turret::SetLevel(int l)
{
	mLevel = l;
	mGun.SetDamage(mBaseDamage + mUpgradeTable[l][0]);
	mGun.SetRate(mBaseRate + mUpgradeTable[l][1]);
	mGun.SetRange(mBaseRange + mUpgradeTable[l][2]);
}

int Turret::GetLevel()
{
	return mLevel;
}

void Turret::LoadAssets()
{
	mGun.LoadAssets();
	mSprite = new Sprite("../Sprites/Turret" + std::to_string(mType+1) + ".png");
	mBaseSprite = new Sprite("../Sprites/Turret1Base2.png");
	mHighlightSprite = new Sprite("../Sprites/TurretHighlight.png");
	mScale = 1;
	mSpriteSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSize = mSpriteSize - sf::Vector2f(4, 4);
	mSprite->setOrigin(mSpriteSize.x / 2, mSpriteSize.y / 2);
	mBaseSprite->setOrigin(mSpriteSize.x / 2, mSpriteSize.y / 2);
	mHighlightSprite->setOrigin((mSpriteSize.x+2) / 2, (mSpriteSize.y+2) / 2);
	ren->Add(mHighlightSprite, mBaseSpriteLayer);
	mHighlightSprite->SetVisible(false);
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
sf::Vector2f Turret::Shoot()
{
	sf::Vector2f hit;
	if (missileAimDelay < 0 || mType != 1)
	{
		float minDist = mRange;
		sf::Vector2f enemy;
		std::vector<sf::Vector2f*> hitPos;
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
			hitPos = mGun.Shoot(mLocation, ang);
			if (hitPos.size() > 0)
			{
				SoundManager::PlaySoundEffect("TurretShoot");
				hit = enemy;
				missileAimDelay = 200;
			}
			mShooting = true;
		}
	}
	else
		missileAimDelay--;
	return hit;
}

void Turret::Draw(sf::Vector2f offset, float scale)
{
	if (mType != 1)
		mGun.Draw(mLocation, offset, scale);
	mHighlightSprite->setScale(scale * mScale * mHScale, scale * mScale * mHScale);
	mHighlightSprite->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset))* scale);
	Entity::Draw(offset, scale);
}

Gun* Turret::GetGun()
{
	return &mGun;
}