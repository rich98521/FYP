#include "stdafx.h"
#include "Entity.h"

Entity::Entity(sf::Vector2f loc, int tSize, Renderer* r)
	:mLocation(loc), mTileSize(tSize), mAngle(0), mBaseAngle(0), mHealth(9), ren(r), mSpriteLayer(5), mBaseSpriteLayer(2)
{
}

void Entity::LoadAssets()
{
	mHealthSprite = new Sprite("../Sprites/Health.png");
	mHealthBarSprite = new Sprite("../Sprites/HealthBar.png");
	ren->Add(mHealthBarSprite, 10);
	ren->Add(mHealthSprite, 10);
	ren->Add(mBaseSprite, mBaseSpriteLayer);
	ren->Add(mSprite, mSpriteLayer);
}

sf::IntRect Entity::Rect() const
{
	return sf::IntRect(mLocation.x, mLocation.y, mSize.x, mSize.y);
}

sf::Vector2f Entity::Location() const
{
	return mLocation;
}

sf::Vector2f Entity::Size() const
{
	return mSize;
}

void Entity::Update(float t)
{
	mLastLocation = mLocation;
	if (abs(mVelocity.x) + abs(mVelocity.y) > 0.00001f)
		mVelocity *= 0.995f - (abs(mAccel.x) + abs(mAccel.y) < 0.00001f ? 0.007f : 0);
	//mVelocity -= mVelocity *2f * t;
	float speed = sqrtf(mVelocity.x * mVelocity.x + mVelocity.y * mVelocity.y);
	distance += speed * t;
	if (distance > fmax(5, speed*t))
	{
		anmFrame++;
		distance -= fmax(5, speed*t);
	}
	//mLocation += mVelocity * t + 0.5f * mAccel * t * t;
	//mVelocity += mAccel * t;
	mVelocity += mAccel * t;
	mLocation += mVelocity * t;
	//mBaseSprite.setRotation(atan2(mVelocity.y, mVelocity.x) / 3.14159f * 180);
	mBaseAngle = (atan2(mVelocity.y, mVelocity.x) / 3.14159f * 180);
}

void Entity::Collision(sf::IntRect e)
{
	if (mLastLocation.x < e.left + (e.width + mSize.x) / 2 &&
		mLastLocation.x > e.left + (-e.width - mSize.x) / 2)
	{
		mLocation.y = (int)(mLocation.y + e.height / 2) / e.width * e.width + (e.width - mSize.x) / 2 * (mVelocity.y / fabs(mVelocity.y));
		mVelocity.y = 0;
	}
	if (mLastLocation.y < e.top + (e.height + mSize.y) / 2 &&
		mLastLocation.y > e.top + (-e.height - mSize.y) / 2)
	{
		mLocation.x = (int)(mLocation.x + e.width / 2) / e.height * e.height + (e.height - mSize.y) / 2 * (mVelocity.x / fabs(mVelocity.x));
		mVelocity.x = 0;
	}
}

void Entity::Draw(sf::Vector2f offset, float scale)
{
	int baseMax = (int)(mBaseSprite->getTexture()->getSize().x) / (int)mSize.y;
	mBaseSprite->setTextureRect(sf::IntRect(mSize.y * ((int)anmFrame % baseMax), 0, mSize.y, mSize.y));
	//t.scale(scale, scale).translate(sf::Vector2f(mLocation)).translate(sf::Vector2f(-offset)).rotate(mBaseAngle).translate(-mSize.x / 2, -mSize.y / 2);
	mBaseSprite->setScale(scale, scale);
	mBaseSprite->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset))* scale);
	mBaseSprite->setRotation(mBaseAngle);
	baseMax = (int)(mSprite->getTexture()->getSize().x) / (int)mSize.y;
	mSprite->setTextureRect(sf::IntRect(mSize.y * ((int)anmFrame % baseMax), 0, mSize.y, mSize.y));
	//t.scale(scale, scale).translate(sf::Vector2f(mLocation)).translate(sf::Vector2f(-offset)).rotate(mAngle).translate(-mSize.x / 2, -mSize.y / 2);
	mSprite->setScale(scale, scale);
	mSprite->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset))* scale);
	mSprite->setRotation(mAngle);
	if (mHealth < 10 && mDrawHealth)
	{
		mHealthSprite->SetVisible(true);
		mHealthBarSprite->SetVisible(true);
		//t.scale(scale, scale).translate(sf::Vector2f(mLocation)).translate(sf::Vector2f(-offset)).translate(-15, -16);
		mHealthBarSprite->setScale(scale, scale);
		mHealthBarSprite->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset) + sf::Vector2f(-15, -16))* scale);
		//t.scale(scale, scale).translate(sf::Vector2f(mLocation)).translate(sf::Vector2f(-offset)).translate(-14, -15).scale(mHealth*2.8f, 1);
		mHealthSprite->setScale(scale*mHealth*2.8f, scale);
		mHealthSprite->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset) + sf::Vector2f(-14, -15))* scale);
	}
	else
	{
		mHealthSprite->SetVisible(false);
		mHealthBarSprite->SetVisible(false);
	}
}

Entity::~Entity()
{
	ren->Remove(mBaseSprite, mBaseSpriteLayer);
	ren->Remove(mSprite, mSpriteLayer);
	ren->Remove(mHealthSprite, 10);
	ren->Remove(mHealthBarSprite, 10);
	delete(mSprite);
	delete(mBaseSprite);
	delete(mHealthBarSprite);
	delete(mHealthSprite);
}