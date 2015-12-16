#include "stdafx.h"
#include "Gun.h"

//gun classed used by enemy, player and turret
Gun::Gun(Renderer* r, int magT)
	:mRen(r), mMagTotal(magT)
{
	mMag = mMagTotal;
	for (int i = 0; i < 10; i++){
		mBulletSprites.push_back(new Sprite("../Sprites/Bullet.png"));
		mBulletSprites[i]->SetVisible(false);
		mRen->Add(mBulletSprites[i], BULLET);
	}
}

//takes position and angle and returns a vector of hit positions
//vector to allow shotgun like guns
//also handles fire rate, accuracy, range and magazine
std::vector<sf::Vector2f*> Gun::Shoot(sf::Vector2f pos, float ang)
{
	mBulletHitPos.clear();
	if (mClock.getElapsedTime().asSeconds() > mRate && mMag != 0)
	{
		mShot = true;
		for (int i = 0; i < mBulletCount; i++)
		{
			ang += (((rand() % 200) - 100) / (100.f)) * (3.14159f  * (1 - mAccuracy));
			mBulletHitPos.push_back(new sf::Vector2f(pos + sf::Vector2f(cos(ang), sin(ang)) * mRange));
			mBulletSprites[i]->setRotation(ang / 3.14159f * 180);
			mBulletSprites[i]->SetVisible(true);
		}
		mDrawRange = mRange;
		mClock.restart();
		mMag--;
	}
	return mBulletHitPos;
}

std::vector<sf::Vector2f*> Gun::GetBullets()
{
	return mBulletHitPos;
}

void Gun::SetRate(float r)
{
	mRate = r;
}

void Gun::Collision(int i, float dist)
{

}

void Gun::Reload()
{
	mMag = mMagTotal;
}

void Gun::SetDrawRange(float range)
{
	mDrawRange = range;
}

//scales, offsets the bullets and handles the bullet timer
void Gun::Draw(sf::Vector2f pos, sf::Vector2f offset, float scale)
{
	if (mClock.getElapsedTime().asSeconds() > 0.06 && mShot)
	{
		for (int i = 0; i < mBulletCount; i++){
			mBulletSprites[i]->SetVisible(false);
		}
		mShot = false;
	}
	for (int i = 0; i < mBulletSprites.size(); i++)
	{
		mBulletSprites[i]->setPosition((pos - offset) * scale);
		mBulletSprites[i]->setScale(mDrawRange * scale, 1 * scale);
	}
}

std::pair<int, int> Gun::GetAmmo()
{
	return std::pair<int, int>(mMag, mMagTotal);
}

Gun::~Gun()
{
	for (int i = 0; i < 10; i++){
		mRen->Remove(mBulletSprites[i], BULLET);
		delete(mBulletSprites[i]);
	}
}