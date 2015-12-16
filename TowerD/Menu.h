#ifndef MENU_H
#define MENU_H
#include "Sprite.h"
#include "Level.h"
#include "Renderer.h"
#include "Text.h"
#include <functional>

//panels are rectangles that can contain buttons, texts, rectangleshapes
//they function as toggleable submenus
class Panel
{
private:
	std::vector<Text*> mTexts;
	std::vector<std::pair<sf::RectangleShape, bool>*> mRects;
	std::pair<sf::RectangleShape, bool> mBackground;
	sf::Vector2i mPos;
	bool mDown = false, mVisible = false;
	Renderer* mRen;
	sf::IntRect mRect;
public:
	Panel() { }
	Panel(Renderer* ren, sf::IntRect r)
		:mRen(ren), mRect(r)
	{
		mBackground.first.setPosition(r.left, r.top);
		mBackground.first.setSize(sf::Vector2f(r.width, r.height));
		mBackground.first.setFillColor(sf::Color(160, 160, 160, 255));
		mBackground.second = false;
		mRen->Add(&mBackground);
	}
	void AddText(Text* t)
	{
		t->setPosition(t->getPosition() + sf::Vector2f(mRect.left, mRect.top));
		mTexts.push_back(t);
		t->SetVisible(false);
		mRen->Add(t);
	}
	void AddRect(std::pair<sf::RectangleShape, bool>* r)
	{
		r->first.setPosition(r->first.getPosition() + sf::Vector2f(mRect.left, mRect.top));
		mRects.push_back(r);
		r->second = false;
		mRen->Add(r);
	}
	void ProcessInput(sf::Event e)
	{
		if (e.type == sf::Event::MouseButtonPressed)
		{
			mDown = true;
		}
		else if (e.type == sf::Event::MouseButtonReleased)
		{
			mDown = false;
		}
	}
	//setting the panel invisible also sets all its members to invsible
	void SetVisible(bool v)
	{
		mVisible = v;
		for each (Text* t in mTexts)
			t->SetVisible(v);
		for each (std::pair<sf::RectangleShape, bool>* r in mRects)
			r->second = v;
		mBackground.second = v;
	}
	bool GetVisible()
	{
		return mVisible;
	}
};

//a scene is a single screen in the menu
class Scene
{
private:
	std::vector<Text*> mTexts;
	std::vector<std::pair<sf::RectangleShape, bool>*> mRects;
	std::vector<Panel*> mPanels;
	Sprite* mBackground;
	sf::Vector2i mPos;
	bool mDown = false;
	Renderer* mRen;
public:
	Scene() { }
	Scene(Renderer* ren, Sprite* s)
		:mBackground(s), mRen(ren)
	{
		ren->Add(mBackground, UI);
		SetVisible(false);
	}
	void ProcessInput(sf::Event e)
	{
		if (e.type == sf::Event::MouseButtonPressed)
		{
			mDown = true;
		}
		else if (e.type == sf::Event::MouseButtonReleased)
		{
			mDown = false;
		}
		for each (Panel* p in mPanels)
			p->ProcessInput(e);
	}
	void AddText(Text* t)
	{
		mTexts.push_back(t);
		t->SetVisible(false);
		mRen->Add(t);
	}
	void AddPanel(Panel* p)
	{
		mPanels.push_back(p);
	}
	void AddRect(std::pair<sf::RectangleShape, bool>* r)
	{
		mRects.push_back(r);
		r->second = false;
		mRen->Add(r);
	}

	std::vector<Panel*> Panels()
	{
		return mPanels;
	}

	void SetVisible(bool v)
	{
		for each (Text* t in mTexts)
			t->SetVisible(v);
		if (!v)
			for each (Panel* p in mPanels)
				p->SetVisible(v);
		for each (std::pair<sf::RectangleShape, bool>* r in mRects)
			r->second = v;
		mBackground->SetVisible(v);
	}
};

class Menu
{
private:
	Level* mLevel;
	sf::Window* mWin;
	std::vector<Text*> mHudTexts;
	std::vector<std::pair<sf::RectangleShape, bool>*> mHudRects;
	sf::Vector2i mPos;
	std::vector<Scene> mScenes;
	int mScene = 0;
	bool mPaused = true;
	void SetPaused(bool);
public:
	Menu(Renderer*, sf::Window*, Level*);
	void Update();
	void ProcessInput(sf::Event);
	void SetScene(int);
	void StartButtonClick();
	int GetScene();
	bool GamePaused();
};

#endif