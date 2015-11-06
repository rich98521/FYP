#include "stdafx.h"
#include "Tile.h"

Tile::Tile(sf::Vector2i loc, int s, int type, Renderer* r) : mType(type), mLocation(loc), ren(r), mSize(s)
{
}

void Tile::LoadAssets()
{
	if (mType == 0)
		mSprite.setTexture("../Sprites/Floor.png");
	else if (mType == 1)
		mSprite.setTexture("../Sprites/Boundary.png");
	else if (mType == 2)
		mSprite.setTexture("../Sprites/Wall.png");
	mSprite.setPosition(mLocation.x, mLocation.y);
	mSprite.setOrigin(mSprite.getTextureRect().width / 2, mSprite.getTextureRect().height / 2);
	ren->Add(&mSprite, 0);
}

sf::IntRect Tile::Rect() const
{
	return sf::IntRect(mLocation.x, mLocation.y, mSize, mSize);
}

int Tile::Type() const
{
	return mType;
}

sf::Vector2i Tile::Location() const
{
	return mLocation;
}

void Tile::Draw(sf::Vector2f offset, float scale)
{
	mSprite.setScale(scale, scale);
	mSprite.setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset))* scale);
}

Tile::~Tile()
{
	ren->Remove(&mSprite, 0);
}