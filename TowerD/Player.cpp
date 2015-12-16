#include "stdafx.h"
#include "Player.h"

Player::Player(sf::Vector2f loc, int tSize, Renderer* r)
	: Entity(loc, tSize, r), mGun(r, 20), mEquipped(0)
{
	mBaseSpriteLayer = CHARACTERBASE;
	mSpriteLayer = CHARACTERTOP;
	mSpawnPos = loc;
	mAimLoc = sf::Vector2i();
}

void Player::LoadAssets()
{
	mCrosshairs = new Sprite("../Sprites/Crosshairs.png");
	mSprite = new Sprite("../Sprites/Player.png");
	mSprite2 = new Sprite("../Sprites/Player.png");
	mBaseSprite = new Sprite("../Sprites/PlayerBase.png");
	mBaseSprite2 = new Sprite("../Sprites/PlayerBase2.png");
	mGhosts.push_back(new Sprite("../Sprites/WallGhost.png"));
	mGhosts.push_back(new Sprite("../Sprites/Turret1Ghost.png"));
	for each (Sprite* s in mGhosts)
	{
		ren->Add(s, GHOST);
		s->setOrigin(16, 16);
		s->SetVisible(false);
	}
	mSprite2->SetVisible(false);
	mSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSpriteSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSprite->setOrigin(mSize.x / 2, mSize.y / 2);
	mSprite2->setOrigin(mSize.x / 2, mSize.y / 2);
	mBaseSprite->setOrigin(mSize.x / 2, mSize.y / 2);
	mBaseSprite2->setOrigin(mSize.x / 2, mSize.y / 2);
	mCrosshairs->setOrigin(mCrosshairs->getTextureRect().height / 2, mCrosshairs->getTextureRect().width / 2);
	ren->Add(mSprite2, ENTITYHIGH);
	ren->Add(mBaseSprite2, mBaseSpriteLayer);
	ren->Add(mCrosshairs, mSpriteLayer);
	Entity::LoadAssets();
}

void Player::ProcessInput(sf::Event e, sf::Vector2f offset, float scale)
{
	if (mAlive)
	{
		if (e.type == sf::Event::KeyPressed){
			if (e.key.code == sf::Keyboard::A)
				dir.x = -1;
			else if (e.key.code == sf::Keyboard::D)
				dir.x = 1;
			else if (e.key.code == sf::Keyboard::W)
				dir.y = -1;
			else if (e.key.code == sf::Keyboard::S)
				dir.y = 1;
			else if (e.key.code == sf::Keyboard::Space)
				mJetpack = true;
			else if (e.key.code == sf::Keyboard::R)
				mGun.Reload();
			//equips selected item and shows/hides placeable ghosts
			else if (e.key.code == sf::Keyboard::Num1)
			{
				if (mEquipped > 0)
					mGhosts[mEquipped - 1]->SetVisible(false);
				mEquipped = 0;
			}
			else if (e.key.code == sf::Keyboard::Num2)
			{
				if (mEquipped > 0)
					mGhosts[mEquipped - 1]->SetVisible(false);
				mEquipped = 1;
				mGhosts[mEquipped - 1]->SetVisible(true);
			}
			else if (e.key.code == sf::Keyboard::Num3)
			{
				if (mEquipped > 0)
					mGhosts[mEquipped - 1]->SetVisible(false);
				mEquipped = 2;
				mGhosts[mEquipped - 1]->SetVisible(true);
			}
		}
		else if (e.type == sf::Event::KeyReleased)
		{
			if (e.key.code == sf::Keyboard::A && dir.x == -1)
				dir.x = 0;
			else if (e.key.code == sf::Keyboard::D && dir.x == 1)
				dir.x = 0;
			else if (e.key.code == sf::Keyboard::W && dir.y == -1)
				dir.y = 0;
			else if (e.key.code == sf::Keyboard::S && dir.y == 1)
				dir.y = 0;
			else if (e.key.code == sf::Keyboard::Space)
				mJetpack = false;
		}
		else if (e.type == sf::Event::MouseMoved)
		{
			mouseLast = sf::Vector2i(e.mouseMove.x, e.mouseMove.y);
		}
		else if (e.type == sf::Event::MouseButtonPressed)
		{
			mDown = true;
		}
		else if (e.type == sf::Event::MouseButtonReleased)
		{
			mDown = false;
		}
		mAccel = sf::Vector2f(0, 0);
		//sets top sprites angle to direction of acceleration
		if (!(dir.x == 0 && dir.y == 0))
		{
			float ang = atan2(dir.y, dir.x);
			mAccel = sf::Vector2f(cos(ang), sin(ang)) * 800.f;
		}
	}
}

void Player::Update(float t, sf::Vector2f offset, float scale)
{
	if (mAlive)
	{
		Entity::Update(t, offset, scale);

		//angles for forwards and sideways leg animations
		//makes sure legs are always facing forward
		mBaseAngle2 = mBaseAngle + 90;
		mBaseAngle2 = mBaseAngle2 > 180 ? mBaseAngle2 - 360 : mBaseAngle2;
		float diff = mBaseAngle - mAngle;
		diff += (diff > 180) ? -360 : (diff < -180) ? 360 : 0;
		mBaseAngle += fabs(diff) > 90 ? 180 : 0;
		mBaseAngle = mBaseAngle > 180 ? mBaseAngle - 360 : mBaseAngle;

		diff = mBaseAngle2 - mAngle;
		diff += (diff > 180) ? -360 : (diff < -180) ? 360 : 0;
		mBaseAngle2 += fabs(diff) > 90 ? 180 : 0;
		mBaseAngle2 = mBaseAngle2 > 180 ? mBaseAngle2 - 360 : mBaseAngle2;

		Aim(offset, scale);
		float ang = mAngle / 180.f * 3.14159f;
		mPlacePos = sf::Vector2i(mLocation + sf::Vector2f(mTileSize / 2, mTileSize / 2) + sf::Vector2f(cos(ang), sin(ang)) * (float)(mTileSize * sqrt(2)));
		mPlacePos.x = (int)(mPlacePos.x / 32) * 32;
		mPlacePos.y = (int)(mPlacePos.y / 32) * 32;

		//player is invulnerable for a few seconds after respawning
		if (invuln)
		{
			float milisec = mInvulnTimer.getElapsedTime().asMilliseconds();
			if (milisec < 2400)
			{
				int i = (int)(milisec / 100);
				SetVisible(i % 2 == 0);
			}
			else
			{
				invuln = false;
				SetVisible(true);
			}
		}
	}
}

void Player::Shoot()
{
	if (mEquipped == 0 && mDown)
		mGun.Shoot(mLocation, mAngle / 180.f * 3.14159f);
}

float Player::JetFuel()
{
	return mJetFuel;
}

void Player::Aim(sf::Vector2f offset, float scale)
{
	//calculates world aim position and sets top sprite to that angle
	if (mAlive){
		mAimLoc = sf::Vector2i((mouseLast.x / scale) + offset.x, (mouseLast.y / scale) + offset.y);
		mAngle = (atan2(mAimLoc.y - mLocation.y, mAimLoc.x - mLocation.x) / 3.14159f * 180);
	}
}

//player has custom hit function to handle respawning
bool Player::Hit(float damage)
{
	if (!invuln)
	{
		Entity::Hit(damage);
		if (!mAlive)
		{
			SetVisible(false);
			mLocation = mSpawnPos;
			mAimLoc = sf::Vector2i(mLocation);
		}
	}
	return true;
}

void Player::Respawn()
{
	mVelocity = sf::Vector2f();
	mAccel = sf::Vector2f();
	invuln = true;
	mInvulnTimer.restart();
	mAlive = true;
	mHealth = 10;
}

Gun* Player::GetGun()
{
	return &mGun;
}

sf::Vector2i* Player::AimPos()
{
	return &mAimLoc;
}

sf::Vector2f* Player::Pos()
{
	return &mLocation;
}

void Player::SetVisible(bool v)
{
	if (mVisible != v)
	{
		mHealthBarSprite->SetVisible(v);
		mHealthSprite->SetVisible(v);
		mBaseSprite->SetVisible(v);
		mBaseSprite2->SetVisible(v);
		mSprite->SetVisible(v);
		mVisible = v;
	}
}

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

//updates
void Player::Draw(sf::Vector2f offset, float scale)
{
	if (mAlive)
		mCrosshairs->setPosition(sf::Vector2f(mouseLast));
	else
		mCrosshairs->setPosition(sf::Vector2f(-10000, -10000));
	for each (Sprite* s in mGhosts)
	{
		s->setPosition((sf::Vector2f(mPlacePos) + sf::Vector2f(-offset))* scale);
		s->setScale(scale, scale);
	}
	mGun.Draw(mLocation, offset, scale);
	Entity::Draw(offset, scale);
	mSprite2->SetVisible(mHeight > mTileSize);
	mSprite2->setPosition(mSprite->getPosition());
	mSprite2->setRotation(mSprite->getRotation());
	mSprite2->setScale(mSprite->getScale());
	int baseMax = (int)(mBaseSprite->getTexture()->getSize().x) / (int)mSize.y;
	float frmBase1 = anmFrame;
	float frmBase2 = anmFrame;
	if (baseMax > 1)
	{
		//works out correct leg animation frames
		//to ensure legs always face forward legs are retracted as player turns
		//and extended again for the sideways walk animation etc
		int half = baseMax / 2;
		int quat = baseMax / 4;
		float diff = mBaseAngle - mAngle;
		diff += (diff > 180) ? -360 : (diff < -180) ? 360 : 0;
		diff /= 90.f;
		int f = (int)anmFrame % baseMax;
		float d = ((f%half) - quat);
		int sign = sgn(d);
		sign = sign == 0 ? 1 : sign;
		float s = (quat - abs(d)) * sign;
		//float s = max((quat - abs(d)) - 1, 0.f) * sign;


		frmBase1 = f + (s * fabs(diff)) + 0.5f;
		frmBase2 = half + f + (s * (1 - fabs(diff))) + 0.5f;


		// diff = mBaseAngle - mAngle;
		//diff += (diff > quat) ? -half : (diff < -quat) ? half : 0;
		//mBaseAngle += fabs(diff) > 90 ? 180 : 0;
	}

	mBaseSprite->setTextureRect(sf::IntRect(mSize.y * ((int)frmBase1 % baseMax), 0, mSize.y, mSize.y));

	mBaseSprite2->setTextureRect(sf::IntRect(mSize.y * ((int)frmBase2 % baseMax), 0, mSize.y, mSize.y));
	mBaseSprite2->setScale(scale * mScale * mHScale, scale * mScale * mHScale);
	mBaseSprite2->setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset)) * scale);
	mBaseSprite2->setRotation(mBaseAngle2);
}

//returns what type of object the player wants to place and where
//returns -1 if player isnt trying to place an object
std::pair<int, sf::Vector2i> Player::GetPlace()
{
	if (mEquipped != 0){
		int t = mDown ? mEquipped : -1;
		mDown = false;
		return std::pair<int, sf::Vector2i>(t, mPlacePos);
	}
}

bool Player::Alive()
{
	return mAlive;
}

Player::~Player()
{
	ren->Remove(mCrosshairs, mBaseSpriteLayer);
	delete(mCrosshairs);
}