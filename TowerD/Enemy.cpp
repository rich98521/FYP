#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(sf::Vector2f loc, int tSize, int t, Renderer* r, std::vector<sf::Vector2f*> players)
	: Entity(loc, tSize, r), mType(t), mGun(r, -1, mType)
{
	mBaseSpriteLayer = CHARACTERBASE;
	mSpriteLayer = CHARACTERTOP;
	mPlayers = players;
	mGun.SetRate(1);
	mGun.SetColor(sf::Color(200, 0, 0, 200));
	canMove = true;
}

void Enemy::LoadAssets()
{
	mGun.LoadAssets();
	mSprite = new Sprite("../Sprites/Enemy2.png");
	mBaseSprite = new Sprite("../Sprites/Enemy1Base.png");
	mSize = sf::Vector2f(22, 22);
	mScale = 0.6;
	mSpriteSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSprite->setOrigin(mSpriteSize.x / 2, mSpriteSize.y / 2);
	mBaseSprite->setOrigin(mSpriteSize.x / 2, mSpriteSize.y / 2);
	Entity::LoadAssets();
}

float ManhDist(float x1, float y1, float x2, float y2)
{
	return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
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

void Enemy::Shoot(sf::Vector2f player, float dist)
{
	mAngle = mBaseAngle;
	if (dist < 96)
	{
		float ang = atan2(player.y - mLocation.y, player.x - mLocation.x);
		mAngle = ang / 3.14159f * 180;
		if(mGun.Shoot(mLocation, ang).size() > 0)
			SoundManager::PlaySoundEffect("EnemyShoot");
	}
}

void Enemy::Draw(sf::Vector2f offset, float scale)
{
	mGun.Draw(mLocation, offset, scale);
	Entity::Draw(offset, scale);
}

bool Enemy::GoalReached()
{
	return mGoalReached;
}

void Enemy::SetPath(std::vector<std::pair<sf::Vector2i, float>> p)
{
	mPath = p;
	//float minDist = FLT_MAX;
	//currentNode = p.size() - 1;
	//for (int i = 1; i < p.size() - 1; i++)
	//{
	//	sf::Vector2f target(mPath[i].first.x * mTileSize, mPath[i].first.y * mTileSize);
	//	float dist = ManhDist(mLocation.x, mLocation.y, target.x, target.y);
	//	if (dist < minDist)
	//	{
	//		minDist = dist;
	//		currentNode = i + 1;
	//		if (dist < (mTileSize-1))
	//			break;
	//	}
	//}
	currentNode = 1;
	mGoalReached = false;
}

Gun* Enemy::GetGun()
{
	return &mGun;
}

Enemy::~Enemy()
{

}