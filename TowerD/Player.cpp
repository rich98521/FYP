#include "stdafx.h"
#include "Player.h"

Player::Player(sf::Vector2f loc, int tSize, Renderer* r)
	: Entity(loc, tSize, r), mGun(r), mEquipped(1)
{
}

void Player::LoadAssets()
{
	mCrosshairs = new Sprite("../Sprites/Crosshairs.png");
	mSprite = new Sprite("../Sprites/Player.png");
	mBaseSprite = new Sprite("../Sprites/PlayerBase.png");
	mGhosts.push_back(new Sprite("../Sprites/WallGhost.png"));
	for each (Sprite* s in mGhosts)
	{
		ren->Add(s, 3);
		s->setOrigin(16, 16);
		s->SetVisible(false);
	}
	mSize = sf::Vector2f(mSprite->getTextureRect().height, mSprite->getTextureRect().height);
	mSprite->setOrigin(mSize.x / 2, mSize.y / 2);
	mBaseSprite->setOrigin(mSize.x / 2, mSize.y / 2);
	mCrosshairs->setOrigin(mCrosshairs->getTextureRect().height / 2, mCrosshairs->getTextureRect().width / 2);
	ren->Add(mCrosshairs, 11);
	Entity::LoadAssets();
}

void Player::ProcessInput(sf::Event e, sf::Vector2f offset, float scale)
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
	mAccel = sf::Vector2f(0,0);
	if (!(dir.x == 0 && dir.y == 0))
	{
		float ang = atan2(dir.y, dir.x);
		mAccel = sf::Vector2f(cos(ang), sin(ang)) * 800.f;
	}
}

void Player::Update(float t, sf::Vector2f offset, float scale)
{
	Entity::Update(t);
	Aim(offset, scale);
	float ang = mAngle / 180.f * 3.14159f;
	mPlacePos = sf::Vector2i(mLocation + sf::Vector2f(mTileSize / 2, mTileSize/2) + sf::Vector2f(cos(ang), sin(ang)) * (float)(mTileSize * sqrt(2)));
	mPlacePos.x = (int)(mPlacePos.x / 32) * 32;
	mPlacePos.y = (int)(mPlacePos.y / 32) * 32;
	if (mEquipped == 1 && mDown)
		mGun.Shoot(mLocation, mAngle / 180.f * 3.14159f, offset, scale);
}

void Player::Aim(sf::Vector2f offset, float scale)
{
	mAimLoc = sf::Vector2i((mouseLast.x / scale) + offset.x, (mouseLast.y / scale) + offset.y);
	mAngle = (atan2(mAimLoc.y - mLocation.y, mAimLoc.x - mLocation.x) / 3.14159f * 180);
}


sf::Vector2i* Player::AimPos()
{
	return &mAimLoc;
}

sf::Vector2f* Player::Pos()
{
	return &mLocation;
}

Gun* Player::GetGun()
{
	return &mGun;
}

void Player::Draw(sf::Vector2f offset, float scale)
{
	mCrosshairs->setPosition(sf::Vector2f(mouseLast));
	for each (Sprite* s in mGhosts)
	{
		s->setPosition((sf::Vector2f(mPlacePos) + sf::Vector2f(-offset))* scale);
		s->setScale(scale, scale);
	}
	mGun.Draw();
	Entity::Draw(offset, scale);
}

std::pair<int, sf::Vector2i> Player::GetPlace()
{
	if (mEquipped != 1){
		int t = mDown ? mEquipped : -1;
		mDown = false;
		return std::pair<int, sf::Vector2i>(t, mPlacePos);
	}
}

Player::~Player()
{
	ren->Remove(mCrosshairs, mBaseSpriteLayer);
	delete(mCrosshairs);
}