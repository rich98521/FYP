#include "stdafx.h"
#include "Tile.h"

//tile class used for all tiles
Tile::Tile(sf::Vector2i loc, int s, int type, Renderer* r) : mType(type), mLocation(loc), ren(r), mSize(s), mLayer(TILE)
{
}

void Tile::LoadAssets()
{
	if (mType == 0)
		mSprite.setTexture("../Sprites/Floor.png");
	else if (mType == 1)
		mSprite.setTexture("../Sprites/Boundary.png");
	else if (mType == 2)
	{
		mSprite.setTexture("../Sprites/Wall.png");
		for (int i = 0; i < 4; i++){
			mCornerSprites[i].setTexture("../Sprites/WallCorner" + std::to_string(i + 1) + ".png");
			mCornerSprites[i].SetVisible(false);
		}
		mCornerSprites[0].setOrigin( mSprite.getTextureRect().width / 2,  mSprite.getTextureRect().height / 2);
		mCornerSprites[1].setOrigin(-mSprite.getTextureRect().width / 2 + 9, mSprite.getTextureRect().height / 2);
		mCornerSprites[2].setOrigin(-mSprite.getTextureRect().width / 2 + 9, -mSprite.getTextureRect().height / 2 + 9);
		mCornerSprites[3].setOrigin(mSprite.getTextureRect().width / 2, -mSprite.getTextureRect().height / 2 + 9);
	}
	mSprite.setPosition(mLocation.x, mLocation.y);
	mSprite.setOrigin(mSprite.getTextureRect().width / 2, mSprite.getTextureRect().height / 2);
	ren->Add(&mSprite, mLayer); 
	if (mType == 2)
		for (int i = 0; i < 4; i++)
			ren->Add(&mCornerSprites[i], mLayer);
}
void Tile::SetEdge(int i, bool b)
{ 
	mEdges[i] = b;
	UpdateCorners();
}

void Tile::UpdateCorners()
{
	mCornerSprites[0].SetVisible(mEdges[1] || mEdges[3]);
	mCornerSprites[1].SetVisible(mEdges[0] || mEdges[3]);
	mCornerSprites[2].SetVisible(mEdges[0] || mEdges[2]);
	mCornerSprites[3].SetVisible(mEdges[2] || mEdges[1]);
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
	for (int i = 0; i < 4; i++)
	{
		mCornerSprites[i].setScale(scale, scale);
		mCornerSprites[i].setPosition((sf::Vector2f(mLocation) + sf::Vector2f(-offset))* scale);
	}
}

void Tile::SetOccupied(bool o)
{
	mOccupied = o;
}

bool Tile::Occupied()
{
	return mOccupied;
}

Tile::~Tile()
{
	ren->Remove(&mSprite, 0);
	for (int i = 0; i < 4; i++)
		ren->Remove(&mCornerSprites[i], 0);
}