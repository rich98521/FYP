#include "stdafx.h"
#include "Gun.h"

//gun class used by enemy, player and turret
Gun::Gun(Renderer* r, int magT, int t)
	:mRen(r), mMagTotal(magT), mType(t)
{
	mMag = mMagTotal;
	if(mType == 0)
		mBulletColor = sf::Color(255, 184, 52, 200);
	else if (mType == 2)
	{
		mBulletColor = sf::Color(255, 52, 52, 200);
		mRate = 0.06f;
	}
	mReloadClock.restart();
}


void Gun::LoadAssets()
{
	for (int i = 0; i < 10; i++){
		mBulletSprites.push_back(new Sprite(mType == 0 || mType == 2 ? "../Sprites/Bullet.png" : mType == 1 ? "../Sprites/Missile.png" : ""));
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
	if (mClock.getElapsedTime().asSeconds() > mRate && mMag != 0 && mReloadClock.getElapsedTime().asSeconds() > mReloadRate)
	{
		mShot = true;
		for (int i = 0; i < mBulletCount; i++)
		{
			float newAng = 0;
			if (mType == 0)
				newAng = ang + (((rand() % 200) - 100) / (100.f)) * (3.14159f  * (1 - mAccuracy));
			mBulletHitPos.push_back(new sf::Vector2f(pos + sf::Vector2f(cos(newAng), sin(newAng)) * mRange));
			mBulletSprites[i]->setColor(mBulletColor);
			mBulletSprites[i]->setRotation(newAng / 3.14159f * 180);
			mBulletSprites[i]->SetVisible(true);
		}
		mDrawRange = mRange;
		mClock.restart();
		mMag--;
	}
	else if (mMag == 0)
		Reload();
	return mBulletHitPos;
}

void Gun::SetColor(sf::Color c)
{
	mBulletColor = c;
}

std::vector<sf::Vector2f*> Gun::GetBullets()
{
	return mBulletHitPos;
}

void Gun::Collision(int i, float dist)
{

}

void Gun::Reload()
{
	if (mMag != mMagTotal)
	{
		mMag = mMagTotal;
		mReloadClock.restart();
		SoundManager::PlaySoundEffect("Reload");
	}
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
			mBulletSprites[i]->SetVisible(false);}
		mShot = false;
	}
	for (int i = 0; i < mBulletSprites.size(); i++)
	{
		mBulletSprites[i]->setPosition((pos - offset) * scale);
		mBulletSprites[i]->setScale(mDrawRange * scale, 1 * scale);
	}
}

void Gun::SetRate(float r)
{
	if (mType != 2)
		mRate = r;
}

void Gun::SetRange(float r)
{
	mRange = r;
}

void Gun::SetAccuracy(float a)
{
	mAccuracy = a;
}

float Gun::GetDamage()
{
	return mDamage;
}

void Gun::SetDamage(float d)
{
	mDamage = d;
}

void Gun::SetMaxAmmo(int a)
{
	mMagTotal = a;
}

float Gun::GetRange()
{
	return mRange;
}

void Gun::SetBulletCount(int c)
{
	mBulletCount = c;
}

std::pair<int, int> Gun::GetAmmo()
{
	return std::pair<int, int>(mMag, mMagTotal);
}

Gun::~Gun()
{
	for (int i = 0; i < mBulletSprites.size(); i++){
		mRen->Remove(mBulletSprites[i], BULLET);
		delete(mBulletSprites[i]);
	}
}