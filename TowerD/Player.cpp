#include "stdafx.h"
#include "Player.h"

Player::Player(sf::Vector2f loc, int tSize, Renderer* r)
	: Entity(loc, tSize, r), mGun(r, 20, 0), mEquipped(0), mSpeed(800),
	mAccTable{ { 0.97f, 0.975f, 0.98f, 0.985f, 0.99f } }, 
	mDamTable{ { 0.4f, 0.47f, 0.54f, 0.61f, 0.68f } }, 
	mRateTable{ { 0.15f, 0.14f, 0.13f, 0.12f, 0.11f } },
	mAmmoTable{ { 22, 25, 28, 32, 36 } },
	mUpgradeCosts{ { 15, 30, 45, 75, 100 } }
{
	mBaseSpriteLayer = CHARACTERBASE;
	mSpriteLayer = CHARACTERTOP;
	mSpawnPos = loc;
	mAimLoc = sf::Vector2i();
	canMove = true;
	mGun.SetDamage(0.4f);
	mCredits = 200;
	mGrenadePouch = 2;
}

void Player::SetId(int i)
{
	Entity::SetId(i);
	mGun.SetOwnerId(i);
}

void Player::LoadAssets()
{
	mGun.LoadAssets();
	mCrosshairs = new Sprite("../Sprites/Crosshairs.png");
	mSprite = new Sprite("../Sprites/Player.png");
	mSprite2 = new Sprite("../Sprites/Player.png");
	mSprite3 = new Sprite("../Sprites/Player2.png");
	mBaseSprite = new Sprite("../Sprites/PlayerBase.png");
	mBaseSprite2 = new Sprite("../Sprites/PlayerBase2.png");
	mGhosts.push_back(new Sprite("../Sprites/WallGhost.png"));
	mGhosts.push_back(new Sprite("../Sprites/Turret1Ghost.png"));
	mGhosts.push_back(new Sprite("../Sprites/Turret2Ghost.png"));
	mGhosts.push_back(new Sprite("../Sprites/Turret3Ghost.png"));
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
	mSprite3->setOrigin(mSize.x / 2, mSize.y / 2);
	mBaseSprite->setOrigin(mSize.x / 2, mSize.y / 2);
	mBaseSprite2->setOrigin(mSize.x / 2, mSize.y / 2);
	mCrosshairs->setOrigin(mCrosshairs->getTextureRect().height / 2, mCrosshairs->getTextureRect().width / 2);
	ren->Add(mSprite2, ENTITYHIGH);
	ren->Add(mSprite3, mSpriteLayer);
	ren->Add(mBaseSprite2, mBaseSpriteLayer);
	ren->Add(mCrosshairs, CROSSHAIRS);
	Entity::LoadAssets();
}

void Player::ProcessInput(sf::Event e, int modifierKeys, sf::Vector2f offset, float scale)
{
	if (mAlive)
	{
		if (e.type == sf::Event::KeyPressed){
			if (e.key.code == sf::Keyboard::A)
				dir.x = -100;
			else if (e.key.code == sf::Keyboard::D)
				dir.x = 100;
			else if (e.key.code == sf::Keyboard::W)
				dir.y = -100;
			else if (e.key.code == sf::Keyboard::S)
				dir.y = 100;
			else if (e.key.code == sf::Keyboard::Space)
				mJetpack = true;
			else if (e.key.code == sf::Keyboard::R)
			{
				mGun.Reload();
			}
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
			else if (e.key.code == sf::Keyboard::Num4)
			{
				if (mEquipped > 0)
					mGhosts[mEquipped - 1]->SetVisible(false);
				mEquipped = 3;
				mGhosts[mEquipped - 1]->SetVisible(true);
			}
			else if (e.key.code == sf::Keyboard::Num5)
			{
				if (mEquipped > 0)
					mGhosts[mEquipped - 1]->SetVisible(false);
				mEquipped = 4;
				mGhosts[mEquipped - 1]->SetVisible(true);
			}
		}
		else if (e.type == sf::Event::KeyReleased)
		{
			if (e.key.code == sf::Keyboard::A && dir.x == -100)
				dir.x = 0;
			else if (e.key.code == sf::Keyboard::D && dir.x == 100)
				dir.x = 0;
			else if (e.key.code == sf::Keyboard::W && dir.y == -100)
				dir.y = 0;
			else if (e.key.code == sf::Keyboard::S && dir.y == 100)
				dir.y = 0;
			else if (e.key.code == sf::Keyboard::Space)
				mJetpack = false;
		}
		else if (e.type == sf::Event::MouseMoved)
		{
			mouseLast = sf::Vector2i(e.mouseMove.x, e.mouseMove.y);
		}
		else if (e.type == sf::Event::JoystickMoved)
		{
			if (e.joystickMove.axis == sf::Joystick::Axis::U)
			{
				int d = 0;
				xDeadZone = true;
				if (e.joystickMove.position > 20){
					d = 1;
					xDeadZone = false;
				}
				else if (e.joystickMove.position < -20){
					d = -1;
					xDeadZone = false;
				}
				if (!yDeadZone)
					d = e.joystickMove.position > 0 ? 1 : -1;
				if (d != 0)
					mJoystickLook.x = (((abs(e.joystickMove.position))*d) / 90.f);
			}
			else if (e.joystickMove.axis == sf::Joystick::Axis::R)
			{
				int d = 0;
				yDeadZone = true;
				if (e.joystickMove.position > 20){
					d = 1;
					yDeadZone = false;
				}
				else if (e.joystickMove.position < -20){
					d = -1;
					yDeadZone = false;
				}
				if (!xDeadZone)
					d = e.joystickMove.position > 0 ? 1 : -1;
				if (d != 0)
					mJoystickLook.y = (((abs(e.joystickMove.position))*d) / 90.f);
			}
			else if (e.joystickMove.axis == sf::Joystick::Axis::X)
			{
				dir.x = 0;
				if (e.joystickMove.position > 25 || e.joystickMove.position < -25)
					dir.x = e.joystickMove.position;
			}
			else if (e.joystickMove.axis == sf::Joystick::Axis::Y)
			{
				dir.y = 0;
				if (e.joystickMove.position > 25||e.joystickMove.position < -25)
					dir.y = e.joystickMove.position;
			}
			else if (e.joystickMove.axis == sf::Joystick::Axis::Z)
			{
				mLDown = false;
				//mRDown = false;
				//if (e.joystickMove.position > 15)
				//	mRDown = true;
				if (e.joystickMove.position < -15)
				{
					if (!triggerDown)
					{
						mLDown = true;
						triggerDown = true;
					}
				}
				else
					triggerDown = false;
			}
			float joyAng = atan2(mJoystickLook.y, mJoystickLook.x);
			float joyDist = sqrt(mJoystickLook.x*mJoystickLook.x + mJoystickLook.y*mJoystickLook.y);
			float dist = 64;
			if (joyDist > .5f)
				dist += (joyDist - .5f) * 192;
			mouseLast = sf::Vector2i((Config::ScreenWidth() / 2) + cos(joyAng) * dist, (Config::ScreenHeight() / 2) + sin(joyAng) * dist);
		}
		else if (e.type == sf::Event::JoystickButtonPressed )
		{
			if (e.joystickButton.button == Controller::LB)
				mRDown = true;
			else if (e.joystickButton.button == Controller::X)
			{
				if (mEquipped > 0)
					mGhosts[mEquipped - 1]->SetVisible(false);
				mEquipped = (mEquipped + 1) % 5;
				if (mEquipped > 0)
					mGhosts[mEquipped - 1]->SetVisible(true);
			}
			else if (e.joystickButton.button == Controller::RB)
				mJetpack = true;
		}
		else if (e.type == sf::Event::JoystickButtonReleased)
		{
			if (e.joystickButton.button == Controller::LB)
				mRDown = false;
			else if (e.joystickButton.button == Controller::RB)
				mJetpack = false;
		}
		else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Button::Left)
		{
			mLDown = true;
		}
		else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Button::Left)
		{
			mLDown = false;
		}
		else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Button::Right)
		{
			mRDown = true;
		}
		else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Button::Right)
		{
			mRDown = false;
		}
		sf::Vector2f acc = sf::Vector2f(0,0);
		//sets top sprites angle to direction of acceleration
		if (!(dir.x == 0 && dir.y == 0))
		{
			float ang = atan2(dir.y, dir.x);
			acc = (sf::Vector2f(dir) / 100.f) * mSpeed;
		}
		SetAcc(acc);
	}
}

bool Player::GetGrenade()
{
	if (mGrenade)
	{
		mGrenade = false;
		return true;
	}
	return false;
}

float Player::GetAimAngle()
{
	return mAngle;
}

void Player::SetDefensePhase(bool b)
{
	mDefensePhase = b;
	if (mDefensePhase)
	{
		mGrenadeCount = mGrenadePouch;
		mHealth = mMaxHealth;
		mGun.Reload();
	}
}

void Player::Update(float t, sf::Vector2f offset, float scale)
{
	if (mAlive)
	{
		Entity::Update(t, offset, scale);
		jetAnmFrame += 0.1;

		//jetpack will only work while player has fuel
		//if jetpack is completely depleted it is disabled till it has been refilled halfway
		if (mJetFuel > mJetFuelMax / 2 && mJetPackEmptied)
			mJetPackEmptied = false;
		float change = ((mJetpack && mJetFuel > 0) && !mJetPackEmptied ? t : -t) * 160;
		//player height increases/decreases depending on jetpack/if theyre on a wall
		float h = mHeight;
		h += mOnWall ? abs(change) : change;
		SetH(fmax(fmin(h, mTileSize + (mJetpack ? 5 : 0)), 0));
		//jetfuel increases/decreases at different rates depending on whether jetpack is in use
		mJetFuel -= change * (mJetpack && !mJetPackEmptied ? 1.5f : 1);
		mJetFuel = fmax(fmin(mJetFuel, mJetFuelMax), 0);
		if (mJetFuel == 0)
			mJetPackEmptied = true;

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

		if (Network::Host() == (mId != 5000))
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
		if (mRDown && mEquipped == 0 && mGrenadeCount > 0 && !mThrown)
		{
			mGrenade = true;
			mThrown = true;
			if (!mDefensePhase)
				mGrenadeCount--;
		}
		if (!mRDown)
			mThrown = false;
	}
}

void Player::Shoot()
{
	if (mEquipped == 0 && mLDown){
		if (mGun.Shoot(mLocation, mAngle / 180.f * 3.14159f).size() > 0)
			SoundManager::PlaySoundEffect("PlayerShoot");
	}
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
		SetAngle(atan2(mAimLoc.y - mLocation.y, mAimLoc.x - mLocation.x) / 3.14159f * 180);
	}
}

void Player::AddCredits(int c)
{
	mCredits += c;
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
			mLDown = false;
			mAimLoc = sf::Vector2i(mLocation);
		}
		else
		{
			if(mHealth < 3)
				SoundManager::PlaySoundEffect("HealthLow");
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
	mSprite3->SetVisible(mHeight > 0 && mJetpack);
	mSprite3->setPosition(mSprite->getPosition());
	mSprite3->setRotation(mSprite->getRotation());
	mSprite3->setScale(mSprite->getScale());
	int baseMax = (int)(mBaseSprite->getTexture()->getSize().x) / (int)mSize.y;
	float frmBase1 = anmFrame;
	float frmBase2 = anmFrame;
	if (baseMax > 1 && (mHeight == 0 || mHeight == mTileSize))
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
	else
	{
		frmBase1 = 0;
		frmBase2 = 0;
	}

	mSprite3->setTextureRect(sf::IntRect(mSize.y * ((int)jetAnmFrame % 4), 0, mSize.y, mSize.y));


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
		int t = mLDown ? mEquipped : mRDown ? 5 : -1;
		mLDown = false;
		mRDown = false;
		return std::pair<int, sf::Vector2i>(t, mPlacePos);
	}
}

bool Player::Alive()
{
	return mAlive;
}

Player::~Player()
{
	ren->Remove(mCrosshairs, CROSSHAIRS);
	ren->Remove(mBaseSprite2, mBaseSpriteLayer);
	ren->Remove(mSprite2, ENTITYHIGH);
	ren->Remove(mSprite3, mSpriteLayer);
	for each (Sprite* s in mGhosts)
	{
		ren->Remove(s, GHOST);
		delete(s);
	}
	delete(mCrosshairs);
	delete(mBaseSprite2);
	delete(mSprite2);
	delete(mSprite3);
}