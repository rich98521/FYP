#ifndef RENDERER_H
#define RENDERER_H
#include "SFML/Graphics.hpp" 
#include "Sprite.h"
#include <set>
#include <map>

struct compare {
	bool operator() (const std::pair<float, Sprite*>& first, const std::pair<float,Sprite*>& second) const
	{
		return (first.first < second.first);
	}
};

class Renderer
{
private:
	std::map<int, float> lastLayers;
	std::set<std::pair<float, Sprite*>, compare> mDrawList;
public:
	Renderer();
	void Add(Sprite*, int);
	void Draw(sf::RenderWindow*);
	void Remove(Sprite*, int);
	void Clear();
};

#endif