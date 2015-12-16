#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(sf::Vector2f loc, int tSize, Renderer* r)
	: Entity(loc, tSize, r), mGun(r)
{
}

void Enemy::LoadAssets()
{
	mSprite = new Sprite("../Sprites/Enemy1.png");
	mBaseSprite = new Sprite("../Sprites/Enemy1Base.png");
	mSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSprite->setOrigin(mSize.x / 2, mSize.y / 2);
	mBaseSprite->setOrigin(mSize.x / 2, mSize.y / 2);
	Entity::LoadAssets();
}

void Enemy::Update(float t, sf::Vector2f offset, float scale)
{
	if (!mGoalReached)
	{
		sf::Vector2f target(mPath[currentNode].first.x * mTileSize, mPath[currentNode].first.y * mTileSize);
		if (ManhDist(mLocation.x, mLocation.y, target.x, target.y) < 80)
		{
			currentNode++;
			if (currentNode < mPath.size())
				target = sf::Vector2f(mPath[currentNode].first.x * mTileSize, mPath[currentNode].first.y * mTileSize);
			else
			{
				mGoalReached = true;
				return;
			}
		}
		float ang = atan2(target.y - mLocation.y, target.x - mLocation.x);
		mAccel = sf::Vector2f(cos(ang), sin(ang)) * 200.f;

		float diff = mPath[currentNode].second - mPath[currentNode - 1].second;
		diff += (diff > pi) ? -(pi * 2) : (diff < -pi) ? (pi * 2) : 0;
		float turnSpeed = 1;
		if (fabs(diff) >= (pi / 2))
			turnSpeed = .6f;
		if (fabs(diff) >= (pi / 4))
			turnSpeed = .8f;
		mSpeed = max(min((ManhDist(mLocation.x, mLocation.y, target.x, target.y)) / 600, 1.f), turnSpeed);

		//mVelocity = sf::Vector2f();

		Entity::Update(t, offset, scale);
	}
}

void Enemy::SetPath(std::vector<std::pair<sf::Vector2i, float>> p)
{
	mPath = p;
	float minDist = FLT_MAX;
	currentNode = p.size() - 1;
	for (int i = 1; i < p.size() - 1; i++)
	{
		sf::Vector2f target(mPath[i].first.x * mTileSize, mPath[i].first.y * mTileSize);
		float dist = ManhDist(mLocation.x, mLocation.y, target.x, target.y);
		if (dist < minDist)
		{
			minDist = dist;
			currentNode = i + 1;
			if (dist < (mTileSize - 1))
				break;
		}
	}
	mGoalReached = false;
}