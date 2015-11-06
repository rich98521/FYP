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

void Enemy::Update(float t, sf::Vector2f core)
{
	float ang = atan2(core.y - mLocation.y, core.x - mLocation.x);
	mAccel = sf::Vector2f(cos(ang), sin(ang)) * 100.f;
	Entity::Update(t);
}