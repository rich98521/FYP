#ifndef MENU_H
#define MENU_H
#include "Sprite.h"
#include "Level.h"
#include "Renderer.h"
#include "Text.h"
#include "Button.h"
#include "SoundManager.h"
#include <functional>

//panels are rectangles that can contain buttons, texts, rectangleshapes
//they function as toggleable submenus
class Panel
{
private:
	std::vector<Button*> mButtons;
	std::vector<Text*> mTexts;
	std::vector<std::pair<sf::RectangleShape, bool>*> mRects;
	std::pair<sf::RectangleShape, bool> mBackground;
	Sprite* mBackgroundImage = 0;
	sf::Vector2i mPos;
	bool mDown = false, mVisible = false, mFocus = false, mCanFocus = true, mEnabled = true;
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
		mRects.push_back(new std::pair<sf::RectangleShape, bool>(sf::RectangleShape(sf::Vector2f(mRect.width, mRect.height)), false));
		mRects[0]->first.setPosition(mRect.left, mRect.top);
		mRects[0]->first.setFillColor(sf::Color(160, 160, 160, 128));
		mRen->Add(mRects[0]);
	}
	Panel(Renderer* ren, Sprite* b)
		:mRen(ren), mBackgroundImage(b)
	{
		mRect = (sf::IntRect)mBackgroundImage->getGlobalBounds();
		mRen->Add(mBackgroundImage, UI);
		mRects.push_back(new std::pair<sf::RectangleShape, bool>(sf::RectangleShape(sf::Vector2f(mRect.width, mRect.height)), false));
		mRects[0]->first.setPosition(mRect.left, mRect.top);
		mRects[0]->first.setFillColor(sf::Color(160, 160, 160, 128));
		mRen->Add(mRects[0]);
	}
	void SetEnabled(bool b)
	{
		mEnabled = b;
		mRects[0]->second = !b;
	}
	void AddButton(Button* b)
	{
		b->Offset(sf::Vector2f(mRect.left, mRect.top));
		mButtons.push_back(b);
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
	bool ProcessInput(sf::Event e)
	{
		if (e.type == sf::Event::MouseButtonPressed)
		{
			mDown = true;
		}
		else if (e.type == sf::Event::MouseButtonReleased)
		{
			mDown = false;
		}
		return mFocus && mCanFocus;
	}
	//used to detect button presses
	void Update(sf::Vector2i m)
	{
		mPos = m;
		for each (Button* b in mButtons)
			b->Update(m, mDown);
		mFocus = false;
		if (mRect.contains(mPos) && mVisible)
			mFocus = true;
	}
	//checks if a button has been pressed and returns pressed button pointer else null
	Button* DownButton()
	{
		Button* button = 0;
		for each (Button* b in mButtons)
			if (b->IsClicked())
			{
			button = b;
			break;
			}
		return button;
	}
	//setting the panel invisible also sets all its members to invsible
	void SetVisible(bool v)
	{
		mVisible = v;
		for each (Text* t in mTexts)
			t->SetVisible(v);
		for each (Button* b in mButtons)
			b->SetVisible(v);
		for each (std::pair<sf::RectangleShape, bool>* r in mRects)
			r->second = v;
		if(v)
			mRects[0]->second = !mEnabled;
		mBackground.second = v;
		if (mBackgroundImage)
			mBackgroundImage->SetVisible(v);
	}
	void SetCanFocus(bool f)
	{
		mCanFocus = f;
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
	std::vector<Button*> mButtons;
	std::vector<Text*> mTexts;
	std::vector<std::pair<sf::RectangleShape, bool>*> mRects;
	std::vector<Panel*> mPanels;
	Sprite* mBackground = 0;
	sf::Vector2i mPos;
	bool mDown = false;
	Renderer* mRen;
	bool mFocus, mVisible, mCanFocus = true;
public:
	Scene() { }
	Scene(Renderer* ren, Sprite* s)
		:mBackground(s), mRen(ren)
	{
		ren->Add(mBackground, UI);
		SetVisible(false);
	}
	Scene(Renderer* ren)
		:mRen(ren)
	{
		SetVisible(false);
	}
	bool ProcessInput(sf::Event e)
	{
		if (e.type == sf::Event::MouseButtonPressed)
		{
			mDown = true;
		}
		else if (e.type == sf::Event::MouseButtonReleased)
		{
			mDown = false;
		}
		mFocus = false;
		for each (Panel* p in mPanels)
			if (p->ProcessInput(e) && mVisible)
				mFocus = true;
		return mFocus || mCanFocus;
	}
	void Update(sf::Vector2i m)
	{
		mPos = m;
		for each (Button* b in mButtons)
			b->Update(m, mDown);
		for each (Panel* p in mPanels)
			p->Update(m);
	}
	void AddButton(Button* b)
	{
		mButtons.push_back(b);
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
	void SetCanFocus(bool f)
	{
		mCanFocus = f;
	}

	Button* DownButton()
	{
		Button* button = 0;
		for each (Button* b in mButtons)
			if (b->IsClicked())
			{
				button = b;
				break;
			}
		for each (Panel* p in mPanels)
			if ((button = p->DownButton()) != 0)
				break;
		return button;
	}

	std::vector<Panel*> Panels()
	{
		return mPanels;
	}

	void SetVisible(bool v)
	{
		for each (Text* t in mTexts)
			t->SetVisible(v);
		for each (Button* b in mButtons)
			b->SetVisible(v);
		if(!v)
			for each (Panel* p in mPanels)
				p->SetVisible(v);
		for each (std::pair<sf::RectangleShape, bool>* r in mRects)
			r->second = v;
		if(mBackground)
			mBackground->SetVisible(v);
		mVisible = v;
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
	bool mFocus = false;
public:
	Menu(Renderer*, sf::Window*, Level*);
	void Update();
	bool ProcessInput(sf::Event);
	void SetScene(int);
	void StartButtonClick();
	int GetScene();
	bool GamePaused();
};

#endif