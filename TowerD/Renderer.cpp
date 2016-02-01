#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer()
{
}

//sprites are stored so they can be rendered by layer
void Renderer::Add(Sprite* s, int l)
{
	if (lastLayers.find(l) == lastLayers.end())
		lastLayers[l] = -1;
	std::pair<float, Sprite*> p(l, s);
	if (p.first <= lastLayers[(int)p.first])
		p.first = lastLayers[(int)p.first] + 0.00001f;
	lastLayers[(int)p.first] = p.first;
	mDrawList.insert(p);
}

void Renderer::Add(Text* t)
{
	mTextDrawList.push_back(t);
}

void Renderer::Add(std::pair<sf::RectangleShape, bool>* r)
{
	mRectDrawList.push_back(r);
}

void Renderer::Remove(Sprite* s, int l)
{
	std::pair<float, Sprite*> p(l, s);
	for (std::set<std::pair<float, Sprite*>>::iterator it = mDrawList.begin(); it != mDrawList.end(); it++)
	{
		if (it->second == p.second)
		{
			mDrawList.erase(*it);
			break;
		}
	}
}

void Renderer::Remove(Text* t)
{
	mTextDrawList.erase(std::find(mTextDrawList.begin(), mTextDrawList.end(), t));
}

void Renderer::Remove(std::pair<sf::RectangleShape, bool>* r)
{
	mRectDrawList.erase(std::find(mRectDrawList.begin(), mRectDrawList.end(), r));
}

void Renderer::Clear()
{
	lastLayers.clear();
	mDrawList.clear();
}

void Renderer::Draw(sf::RenderWindow* window)
{
	std::set<std::pair<float, Sprite*>>::iterator it;
	for (it = mDrawList.begin(); it != mDrawList.end(); it++)
		if(it->second->Visible())
			window->draw(*it->second);

	for each(std::pair<sf::RectangleShape, bool>* r in mRectDrawList)
		if (r->second)
			window->draw(r->first);

	for each(Text* t in mTextDrawList)
		if (t->Visible())
			window->draw(*t);
}
