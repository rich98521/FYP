#ifndef TILE_H
#define TILE_H
#include "Sprite.h"
#include "Renderer.h"

class Tile
{
protected:
	sf::Vector2i mLocation;
	Sprite mSprite;
	Renderer* ren;
	int mType, mSize, mLayer;
	bool mOccupied = false;
	bool mEdges[4];
	Sprite mCornerSprites[4];
	void UpdateCorners();

public:
	Tile(sf::Vector2i, int, int, Renderer* r);
	Tile(){};
	void LoadAssets();
	sf::IntRect Rect() const;
	sf::Vector2i Location() const;
	int Type() const;
	virtual void Draw(sf::Vector2f, float);
	void SetOccupied(bool);
	bool Occupied();
	void SetEdge(int, bool);
	~Tile();
};


#endif