#ifndef ENTITY_H
#define ENTITY_H
#include "Tile.h"
#include "Sprite.h"
#include "Network.h"
#include "Renderer.h"
#include "Gun.h"


class Entity
{
protected:
	sf::Vector2f mLocation;
	sf::Vector2f mSize;
	sf::Vector2f mSpriteSize;
	sf::Vector2f mLastLocation;
	sf::Vector2f mVelocity;
	sf::Vector2f mAccel;
	Renderer* ren;
	Sprite* mBaseSprite;
	Sprite* mSprite;
	Sprite* mHealthSprite;
	Sprite* mHealthBarSprite;
	const float pi = 3.1415926535f;
	const float toDeg = 1 / 180 * pi;
	const float toRad = 1 / pi * 180;
	float mAngle = 0, mBaseAngle = 0, distance = 0;
	bool mMoving;
	bool mDrawHealth = true;
	float anmFrame = 0;
	const int mTileSize;
	float mHealth, mMaxHealth;
	int mSpriteLayer, mBaseSpriteLayer;
	float mSpeed = 1;
	float mScale = 1;
	bool mAlive = true;
	float mHScale = 1;
	float mHeight = 0;
	bool mOnWall = false, canMove = false;
	bool mCollision = true;
	float mCoRestitution = 0;
	int mId = 0;
	bool mAngleChange, mAccVelChange;
	float mLastAngle;
	void SetAngle(float a){ if (a != mAngle && abs(mAngle - mLastAngle) > 5 ){ mAngleChange = true; mLastAngle = mAngle; } mAngle = a; }
	void SetBaseAngle(float a){ if (a != mBaseAngle){ mAngleChange = true; } mBaseAngle = a; }
	void SetAcc(sf::Vector2f a){ if (a != mAccel){ mAccVelChange = true; } mAccel = a; }
	void SetVel(sf::Vector2f v){ if (v != mVelocity){ mAccVelChange = true; } mVelocity = v; }
	void SetH(float h){ if (h != mHeight){ mAccVelChange = true; } mHeight = h; }

public:
	Entity(sf::Vector2f, int, Renderer*);
	Entity() :mTileSize(0){};
	virtual void Update(float, sf::Vector2f, float);
	virtual void LoadAssets();
	void Collision(sf::IntRect);
	bool Collision(sf::Vector2f, float);
	sf::Vector2f Location() const;
	sf::Vector2f Size() const;
	virtual void Draw(sf::Vector2f, float);
	sf::IntRect Rect() const;
	virtual bool Hit(float);
	virtual Gun* GetGun(){ return NULL; }
	float Health();
	float Height();
	void SetOnWall(bool);
	bool CanCollide(){ return mCollision; }
	sf::Vector2f GetVelocity(){ return mVelocity; }
	sf::Vector2f GetAccel(){ return mAccel; }
	bool Alive(){ return mAlive; };
	float GetRestitution(){ return mCoRestitution; }
	int GetId(){ return mId; }
	virtual void SetId(int);
	void SetPosVelAcc(sf::Vector2f p, sf::Vector2f v, sf::Vector2f a){ mLocation = p; mVelocity = v; mAccel = a; }
	void SetHeight(float h){ mHeight = h; }
	void SetAng(float a){ mAngle = a; }
	void SetBaseAng(float a){ mBaseAngle = a; }
	float GetAng(){ return mAngle; }
	float GetBaseAng(){ return mBaseAngle; }
	bool AngChange(){ bool b = mAngleChange; mAngleChange = false; return b; }
	bool AccVelChange(){ bool b = mAccVelChange; mAccVelChange = false; return b; }
	~Entity();
};


#endif