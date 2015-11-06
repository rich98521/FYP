#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer()
{
}

void Renderer::Add(Sprite* s, int l)
{
	if (lastLayers.find(l) == lastLayers.end())
		lastLayers[l] = -1;
	std::pair<float, Sprite*> p(l, s);
	if (p.first <= lastLayers[(int)p.first])
		p.first = lastLayers[(int)p.first] + 0.001f;
	lastLayers[(int)p.first] = p.first;
	mDrawList.insert(p);
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

void Renderer::Clear()
{
	lastLayers.clear();
	mDrawList.clear();
}

void Renderer::Draw(sf::RenderWindow* window)
{
	//mDrawList.sort(compare);
	std::set<std::pair<float, Sprite*>>::iterator it;
	for (it = mDrawList.begin(); it != mDrawList.end(); it++)
	{
		if(it->second->Visible())
			window->draw(*it->second);
	}
	//for each (std::pair<int, std::pair<Sprite*, sf::RenderStates>> v in mDrawList)
	//{
	//	window->draw(*v.second.first, v.second.second);
	//}
}
