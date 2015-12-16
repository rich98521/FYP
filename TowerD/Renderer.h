#ifndef RENDERER_H
#define RENDERER_H
#include "Sprite.h"
#include "Text.h"
#include <set>
#include <map>

struct compare {
	bool operator() (const std::pair<float, Sprite*>& first, const std::pair<float, Sprite*>& second) const
	{
		return (first.first < second.first);
	}
};

class Renderer
{
private:
	std::map<int, float> lastLayers;
	std::set<std::pair<float, Sprite*>, compare> mDrawList;
	std::vector<Text*> mTextDrawList;
	std::vector<std::pair<sf::RectangleShape, bool>*> mRectDrawList;
public:
	Renderer();
	void Add(Sprite*, int);
	void Add(Text*);
	void Add(std::pair<sf::RectangleShape, bool>*);
	void Draw(sf::RenderWindow*);
	void Remove(Sprite*, int);
	void Remove(Text*);
	void Remove(std::pair<sf::RectangleShape, bool>*);
	void Clear();
};

#endif