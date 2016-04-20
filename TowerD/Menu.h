#ifndef MENU_H
#define MENU_H
#include "Sprite.h"
#include "Level.h"
#include "Renderer.h"
#include "Text.h"
#include "Button.h"
#include "Slider.h"
#include "TextBox.h"
#include "SoundManager.h"
#include <functional>
#include "Network.h"

//panels are rectangles that can contain buttons, texts, rectangleshapes
//they function as toggleable submenus
enum SceneNum {
	MAIN,
	LEVELS,
	GAME,
	OPTIONS,
	SERVERS
};


class Panel
{
private:
	std::vector<Button*> mButtons;
	std::vector<Text*> mTexts;
	std::vector<std::pair<sf::RectangleShape, bool>*> mRects;
	std::pair<sf::RectangleShape, bool> mBackground;
	Sprite* mBackgroundImage = 0;
	sf::Vector2i mPos;
	bool mDown = false, mVisible = false, mFocus = false, mCanFocus = true, mEnabled = true, mJoyFocus = false;
	Renderer* mRen;
	sf::FloatRect mRect;
	int mType = 0;
public:
	Panel() { }
	Panel(Renderer* ren, sf::FloatRect r)
		:mRen(ren), mRect(r)
	{
		mType = 0;
		mBackground.first.setPosition(r.left, r.top);
		mBackground.first.setSize(sf::Vector2f(r.width, r.height));
		mBackground.first.setFillColor(sf::Color(50, 50, 50, 255));
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
		mType = 1;
		mRect = mBackgroundImage->getGlobalBounds();
		mRen->Add(mBackgroundImage, UI);
		mRects.push_back(new std::pair<sf::RectangleShape, bool>(sf::RectangleShape(sf::Vector2f(mRect.width, mRect.height)), false));
		mRects[0]->first.setPosition(mRect.left, mRect.top);
		mRects[0]->first.setFillColor(sf::Color(160, 160, 160, 128));
		mRen->Add(mRects[0]);
	}

	void SetPosition(sf::Vector2f p)
	{
		float xChange = p.x - mRect.left;
		float yChange = p.y - mRect.top;
		mRect.left = p.x;
		mRect.top = p.y;
		if (mType == 0)
		{
			mBackground.first.setPosition(mRect.left, mRect.top);
		}
		for each (std::pair<sf::RectangleShape, bool>* r in mRects)
		{
			r->first.setPosition(r->first.getPosition() + sf::Vector2f(xChange, yChange));
		}
		for each (Button* b in mButtons)
		{
			b->Offset(xChange, yChange);
		}
		for each (Text* t in mTexts)
		{
			t->setPosition(t->getPosition() + sf::Vector2f(xChange, yChange));
		}
	}

	~Panel()
	{
		for each(Button* b in mButtons)
			delete(b);
		mButtons.clear();
		for each(Text* t in mTexts)
		{
			mRen->Remove(t);
			delete(t);
		}
		mTexts.clear();
		for each(std::pair<sf::RectangleShape, bool>* r in mRects)
		{
			mRen->Remove(r);
			delete(r);
		}
		mRects.clear();
		mRen->Remove(mBackgroundImage, UI);
		mRen->Remove(&mBackground);
		delete(mBackgroundImage);
	}
	std::vector<Button*>* Buttons()
	{
		return &mButtons;
	}
	void SetEnabled(bool b)
	{
		mEnabled = b;
		mRects[0]->second = !b;
	}
	bool Enabled()
	{
		return mEnabled;
	}
	void SetJoystickCanFocus(bool b)
	{
		mJoyFocus = b;
	}
	bool JoystickCanFocus()
	{
		return mJoyFocus;
	}
	void AddButton(Button* b)
	{
		b->Offset(sf::Vector2f(mRect.left, mRect.top));
		b->SetPanel(this);
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
	bool ProcessInput(sf::Event e, int modifierKeys)
	{
		if (e.type == sf::Event::MouseButtonPressed)
		{
			mDown = true;
		}
		else if (e.type == sf::Event::MouseButtonReleased)
		{
			mDown = false;
		}
		else if (e.type == sf::Event::JoystickButtonPressed)
		{
			if (e.joystickButton.button == 0)
			{
				mDown = true;
			}
		}
		else if (e.type == sf::Event::JoystickButtonReleased)
		{
			if (e.joystickButton.button == 0)
			{
				mDown = false;
			}
		}
		return mFocus && mCanFocus;
	}
	//used to detect button presses
	void Update(sf::Vector2i m, int selected)
	{
		mPos = m;
		int i = 0;
		for each (Button* b in mButtons)
		{
			b->Update(m, mDown, i == selected);
			i++;
		}
		mFocus = false;
		if (mRect.contains(sf::Vector2f(mPos)) && mVisible)
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
	Button* SelectedButton()
	{
		Button* button = 0;
		for each (Button* b in mButtons)
			if (b->IsSelected())
			{
			button = b;
			break;
			}
		return button;
	}
	Button* UnSelectedButton()
	{
		Button* button = 0;
		for each (Button* b in mButtons)
			if (b->UnSelected())
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
	std::vector<Slider*> mSliders;
	std::vector<TextBox*> mTextBoxes;
	std::vector<Text*> mTexts;
	std::vector<std::pair<sf::RectangleShape, bool>*> mRects;
	std::vector<Panel*> mPanels;
	Sprite* mBackground = 0;
	std::pair<sf::RectangleShape, bool> mBackgroundRect;
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
	Scene(Renderer* ren, bool bg)
		:mRen(ren)
	{
		if (bg)
		{
			mBackgroundRect =std::pair<sf::RectangleShape, bool>(sf::RectangleShape(sf::Vector2f(1200, 900)), false);
			mBackgroundRect.first.setPosition(sf::Vector2f(0, 0));
			ren->Add(&mBackgroundRect);
		}
		SetVisible(false);
	}
	bool ProcessInput(sf::Event e, int modifierKeys)
	{
		mFocus = false;
		if (e.type == sf::Event::MouseButtonPressed)
		{
			mDown = true;
		}
		else if (e.type == sf::Event::MouseButtonReleased)
		{
			mDown = false;
		}
		else if (e.type == sf::Event::JoystickButtonPressed)
		{
			if (e.joystickButton.button == 0)
			{
				mDown = true;
			}
		}
		else if (e.type == sf::Event::JoystickButtonReleased)
		{
			if (e.joystickButton.button == 0)
			{
				mDown = false;
			}
		}
		else if (e.type == sf::Event::JoystickMoved)
		{
			for each (Panel* p in mPanels)
				if (p->GetVisible() && p->Buttons()->size()>0 && p->JoystickCanFocus())
				{
				mFocus = true;
				break;
				}
		}
		else if (e.type == sf::Event::KeyPressed)
		{
			for each (TextBox* t in mTextBoxes)
				t->InputKey(e.key.code, modifierKeys);
		}
		for each (Panel* p in mPanels)
			if (p->ProcessInput(e, modifierKeys) && mVisible)
				mFocus = true;
		return mFocus || mCanFocus;
	}
	void Update(sf::Vector2i m, int selected)
	{
		mPos = m;
		int i = 0;
		for each (Button* b in mButtons)
		{
			b->Update(m, mDown, i == selected);
			i++;
		}
		for each (Slider* s in mSliders)
			s->Update(m, mDown);
		for each (TextBox* t in mTextBoxes)
			t->Update(m, mDown);
		for each (Panel* p in mPanels)
		{
			p->Update(m, selected - i);
			i += p->Buttons()->size();
		}
	}
	std::vector<Button*>* GetButtons()
	{
		return &mButtons;
	}
	void AddButton(Button* b)
	{
		mButtons.push_back(b);
	}
	void AddSlider(Slider* s)
	{
		mSliders.push_back(s);
	}
	void AddTextBox(TextBox* t)
	{
		mTextBoxes.push_back(t);
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
	void SetBackColor(sf::Color c)
	{
		mBackgroundRect.first.setFillColor(c);
	}

	void ClearPanels()
	{
		for each (Panel* p in mPanels)
			delete(p);
		mPanels.clear();
	}

	void ClearRects()
	{
		for each (std::pair<sf::RectangleShape, bool>* r in mRects)
		{
			mRen->Remove(r);
			delete(r);
		}
		mRects.clear();
	}

	void ClearTexts()
	{
		for each (Text* t in mTexts)
		{
			mRen->Remove(t);
			delete(t);
		}
		mTexts.clear();
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

	Button* SelectedButton()
	{
		Button* button = 0;
		for each (Button* b in mButtons)
			if (b->IsSelected())
			{
			button = b;
			break;
			}
		for each (Panel* p in mPanels)
			if ((button = p->SelectedButton()) != 0)
				break;
		return button;
	}

	Button* UnSelectedButton()
	{
		Button* button = 0;
		for each (Button* b in mButtons)
			if (b->UnSelected())
			{
			button = b;
			break;
			}
		for each (Panel* p in mPanels)
			if ((button = p->UnSelectedButton()) != 0)
				break;
		return button;
	}

	Slider* ChangedSlider()
	{
		Slider* slider = 0;
		for each (Slider* s in mSliders)
			if (s->ValueChanged())
			{
			slider = s;
			break;
			}
		return slider;
	}

	TextBox* ChangedTextBox()
	{
		TextBox* textBox = 0;
		for each (TextBox* t in mTextBoxes)
			if (t->ValueChanged())
			{
			textBox = t;
			break;
			}
		return textBox;
	}

	std::vector<Panel*>* Panels()
	{
		return &mPanels;
	}


	std::vector<TextBox*>* TextBoxes()
	{
		return &mTextBoxes;
	}

	std::vector<std::pair<sf::RectangleShape, bool>*>* Rects()
	{
		return &mRects;
	}

	std::vector<Text*>* Texts()
	{
		return &mTexts;
	}

	void SetVisible(bool v)
	{
		for each (Text* t in mTexts)
			t->SetVisible(v);
		for each (Button* b in mButtons)
			b->SetVisible(v);
		for each (Slider* s in mSliders)
			s->SetVisible(v);
		for each (TextBox* t in mTextBoxes)
			t->SetVisible(v);
		if(!v)
			for each (Panel* p in mPanels)
				p->SetVisible(v);
		for each (std::pair<sf::RectangleShape, bool>* r in mRects)
			r->second = v;
		if(mBackground)
			mBackground->SetVisible(v);
		else
			mBackgroundRect.second = v;
		mVisible = v;
	}
};

class Menu
{
private:
	Level* mLevel;
	sf::Window* mWin;
	std::vector<Text*> mHudTexts;
	std::vector<Button*> mHudButtons;
	std::map<Panel*, Turret*> mTurretPanels;
	std::vector<std::pair<sf::RectangleShape, bool>*> mHudRects;
	std::vector<Button*>* mSelectedButtons;
	int mSelectedButton;
	sf::Vector2i mPos;
	std::vector<Scene*> mScenes;
	int mScene = 0, mPreviousScene;
	bool mPaused = true;
	void SetPaused(bool);
	bool mFocus = false;
	void InitHud();
	Renderer* mRen;
	void UpdateTurretMenus();
	Camera* mCam;
	bool mTurretPanelsVis, mPauseMenu;
	bool mXAxisReset, mYAxisReset, mAxisReset;
	Turret* mSelectedTurret;
	Text* mCountdown;

public:
	Menu(Renderer*, sf::Window*, Level*, Camera*);
	void Update();
	bool ProcessInput(sf::Event, int);
	void SetScene(int);
	int GetScene();
	bool GamePaused();
};

#endif