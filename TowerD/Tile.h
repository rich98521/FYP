#ifndef TILE_H
#define TILE_H
#include "SFML/Graphics.hpp" 
#include "Sprite.h"
#include "Renderer.h"

class Tile
{
protected:
	sf::Vector2i mLocation;
	Sprite mSprite;
	Renderer* ren;
	int mType, mSize;
public:
	Tile(sf::Vector2i, int, int, Renderer* r);
	Tile(){};
	void LoadAssets();
	sf::IntRect Rect() const;
	sf::Vector2i Location() const;
	int Type() const;
	virtual void Draw(sf::Vector2f, float);
	~Tile();
};


#endif