#include "stdafx.h"
#include "Entity.h"

//base class for all entities in the game
//anything you can interact with in game that isnt a tile
//contains functions common to most if not all entities
Entity::Entity(sf::Vector2f loc, int tSize, Renderer* r)
	:mLocation(loc), mTileSize(tSize), mAngle(0), mBaseAngle(0), mHealth(10), mMaxHealth(10), ren(r), mSpriteLayer(ENTITYTOP), mBaseSpriteLayer(ENTITYBASE), mCollision(true)
{
}

void Entity::LoadAssets()
{
	mHealthSprite = new Sprite("../Sprites/Health.png");
	mHealthBarSprite = new Sprite("../Sprites/HealthBar.png");
	ren->Add(mHealthBarSprite, UI);
	ren->Add(mHealthSprite, UI);
	ren->Add(mBaseSprite, mBaseSpriteLayer);
	ren->Add(mSprite, mSpriteLayer);
}

void Entity::SetId(int i)
{
	mId = i;
}

//damages health and returns bool indicating if the shot
//killed the entity
bool Entity::Hit(float damage)
{
	if (mHealth > 0)
	{
		mHealth -= damage;
		if (mHealth <= 0)
		{
			mHealth = 0;
			mAlive = false;
		}
		if (Network::Host())
		{
			EntityDamagePacket packet;
			packet.id = mId;
			packet.health = mHealth;
			sf::Packet p = sf::Packet();
			p << packet;
			Network::SendUdp(p);
		}
	}
	return mAlive;
}

sf::IntRect Entity::Rect() const
{
	return sf::IntRect(mLocation.x, mLocation.y, mSize.x*mScale, mSize.y*mScale);
}

sf::Vector2f Entity::Location() const
{
	return mLocation;
}

sf::Vector2f Entity::Size() const
{
	return mSize * mScale;
}

float Dot(sf::Vector2f v1, sf::Vector2f v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float Entity::Health()
{
	return mHealth;
}

float Entity::Height()
{
	return mHeight;
}

void Entity::SetOnWall(bool b)
{
	mOnWall = b;
}

//calculates new position using acceleration and velocity
//also handles animation timing
void Entity::Update(float t, sf::Vector2f offset, float scale)
{
	if (canMove)
	{
		mLastLocation = mLocation;
		if (abs(mVelocity.x) + abs(mVelocity.y) > 0.00001f)
			mVelocity *= 0.995f - (abs(mAccel.x) + abs(mAccel.y) < 0.00001f ? 0.009f : 0);
		float speed = sqrtf(mVelocity.x * mVelocity.x + mVelocity.y * mVelocity.y);
		distance += speed * t;
		if (distance > fmax(5, speed*t))
		{
			anmFrame++;
			distance -= fmax(5, speed*t);
		}
		mVelocity += mAccel * t;
		mLocation += mVelocity * t * mSpeed;
		mBaseAngle = atan2(mVelocity.y, mVelocity.x) / 3.14159f * 180;
		mHScale = 1 + (mHeight / mTileSize) * .15f;
	}
}

//collision with tile response used by all entities
void Entity::Collision(sf::IntRect e)
{
	//checks if colliding vertically
	if (mLastLocation.x < e.left + (e.width + Size().x) / 2 &&
		mLastLocation.x > e.left + (-e.width - Size().x) / 2)
	{
		//moves entity to closest tile edge along y
		mLocation.y = (int)(mLocation.y + e.height / 2) / e.width * e.width + ((e.height - Size().y) / 2 - 0.001f) * (mVelocity.y / fabs(mVelocity.y));
		SetVel(sf::Vector2f(mVelocity.x, mVelocity.y * -1 * mCoRestitution));
	}
	//checks if colliding horizontally
	if (mLastLocation.y < e.top + (e.height + Size().y) / 2 &&
		mLastLocation.y > e.top + (-e.height - Size().y) / 2)
	{
		//moves entity to closest tile edge along x
		mLocation.x = (int)(mLocation.x + e.width / 2) / e.height * e.height + ((e.width - Size().x) / 2 - 0.001f)* (mVelocity.x / fabs(mVelocity.x));
		SetVel(sf::Vector2f(mVelocity.x * -1 * mCoRestitution, mVelocity.y));
	}
}

//collision with entity response used by all entities
bool Entity::Collision(sf::Vector2f pos, float size)
{
	if (canMove)
	{
		sf::Vector2f diff = pos - mLocation;
		if (abs(diff.x + diff.y) <= (size * 2) + (Size().x / 3.f))
		{
			float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
			float intersect = (size + (Size().x / 3.f)) - dist;
			if (intersect > 0)
			{
				if (dist == 0)
					diff = sf::Vector2f(0, -1);
				else
					diff /= dist;
				mLocation -= diff * (intersect / 2.f);
				SetVel(mVelocity - (diff * (intersect / 20.f)));
				return true;
			}
		}
	}
	return false;
}

//drawing is handled by the renderer
//this function is used to update the sprites with the camera offset and scale
void Entity::Draw(sf::Vector2f offset, float scale)
{
	int baseMax = (int)(mBaseSprite->getTexture()->getSize().x) / (int)mSpriteSize.y;
	mBaseSprite->setTextureRect(sf::IntRect(mSpriteSize.y * ((int)anmFrame % baseMax), 0, mSpriteSize.y, mSpriteSize.y));
	mBaseSprite->setScale(scale * mScale * mHScale, scale * mScale * mHScale);
	mBaseSprite->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset))* scale);
	mBaseSprite->setRotation(mBaseAngle);
	baseMax = (int)(mSprite->getTexture()->getSize().x) / (int)mSpriteSize.y;
	mSprite->setTextureRect(sf::IntRect(mSpriteSize.y * ((int)anmFrame % baseMax), 0, mSpriteSize.y, mSpriteSize.y));
	mSprite->setScale(scale * mScale * mHScale, scale * mScale * mHScale);
	mSprite->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset))* scale);
	mSprite->setRotation(mAngle);
	if (mHealth < mMaxHealth && mDrawHealth)
	{
		mHealthSprite->SetVisible(true);
		mHealthBarSprite->SetVisible(true);
		mHealthBarSprite->setScale(scale, scale);
		mHealthBarSprite->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset) + sf::Vector2f(-15, -16))* scale);
		mHealthSprite->setScale(scale * (mHealth / mMaxHealth) * 28.f, scale);
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