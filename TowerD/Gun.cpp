#include "stdafx.h"
#include "Gun.h"

Gun::Gun(Renderer* r)
	:mRen(r)
{
	for (int i = 0; i < 10; i++){
		mBulletSprites.push_back(new Sprite("../Sprites/Bullet.png"));
		mBulletSprites[i]->SetVisible(false);
		mRen->Add(mBulletSprites[i], 4);
	}
}

std::vector<sf::Vector2f> Gun::Shoot(sf::Vector2f pos, float ang, sf::Vector2f offset, float scale)
{
	mBulletHitPos.clear();
	if (mClock.getElapsedTime().asSeconds() > mRate && mMag > 0)
	{
		mShot = true;
		for (int i = 0; i < mBulletCount; i++)
		{
			ang += (((rand() % 200) - 100) / (100.f)) * (3.14159f  * (1 - mAccuracy));
			mBulletHitPos.push_back(pos + sf::Vector2f(cos(ang), sin(ang)) * mRange);
			mBulletSprites[i]->setPosition((pos - offset) * scale);
			mBulletSprites[i]->setScale(mRange * scale, 1);
			mBulletSprites[i]->setRotation(ang / 3.14159f *180);
			mBulletSprites[i]->SetVisible(true);
		}
		mClock.restart();
		mMag--;
	}
	return mBulletHitPos;
}

std::vector<sf::Vector2f> Gun::GetBullets() const
{
	return mBulletHitPos;
}

void Gun::Collision(int i, float dist)
{

}

void Gun::Reload()
{
	mMag = mMagTotal;
}

void Gun::Draw()
{
	if (mClock.getElapsedTime().asSeconds() > 0.1 && mShot)
	{
		for (int i = 0; i < mBulletCount; i++)
			mBulletSprites[i]->SetVisible(false);
		mShot = false;
	}
}