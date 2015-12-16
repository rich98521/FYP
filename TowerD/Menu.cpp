#include "stdafx.h"
#include "Menu.h"


void Menu::StartButtonClick()
{
	SetScene(1);
}

Menu::Menu(Renderer* ren, sf::Window* win, Level* l)
	:mLevel(l), mWin(win)
{
	//sets up menu items (buttons, texts, scenes, rectangleshapes, panels)
	mScenes.push_back(Scene(ren, new Sprite("../Sprites/MainMenu.png")));
	Text* t = new Text("Robot Defense", "DISTGRG_.ttf");
	t->setCharacterSize(100);
	t->setPosition(200, 80);
	t->setColor(sf::Color(255, 0, 0, 255));
	mScenes.back().AddText(t);
	mScenes.push_back(Scene(ren, new Sprite("../Sprites/LevelSelect.png")));
	mScenes.push_back(Scene(ren, new Sprite("../Sprites/Hud.png")));
	mHudRects.push_back(new std::pair<sf::RectangleShape, bool>);
	mHudRects.back()->first = sf::RectangleShape(sf::Vector2f(148, 24));
	mHudRects.back()->first.setPosition(6, 842);
	mHudRects.back()->first.setFillColor(sf::Color(21, 249, 39, 200));
	mScenes.back().AddRect(mHudRects.back());
	mHudRects.push_back(new std::pair<sf::RectangleShape, bool>);
	mHudRects.back()->first = sf::RectangleShape(sf::Vector2f(148, 24));
	mHudRects.back()->first.setPosition(6, 870);
	mHudRects.back()->first.setFillColor(sf::Color(21, 39, 125, 200));
	mScenes.back().AddRect(mHudRects.back());
	mHudTexts.push_back(new Text("test", "detente.ttf"));
	mHudTexts.back()->setPosition(1100, 864);
	mHudTexts.back()->setColor(sf::Color(64, 64, 64, 240));
	mHudTexts.back()->setCharacterSize(15);
	mScenes.back().AddText(mHudTexts.back());
	mScenes.back().AddPanel(p);

	SetScene(0);
}

void Menu::ProcessInput(sf::Event e)
{
	if (e.type == sf::Event::MouseMoved)
	{
		mPos = sf::Vector2i(e.mouseMove.x, e.mouseMove.y);
	}
	if (mScene == 2)
	{
		if (e.type == sf::Event::KeyReleased)
		{
			if (e.key.code == sf::Keyboard::Escape)
			{
				//toggles pause menu panel
				mScenes[mScene].Panels()[0]->SetVisible(!mScenes[mScene].Panels()[0]->GetVisible());
				SetPaused(mScenes[mScene].Panels()[0]->GetVisible());
			}
		}
	}
	mScenes[mScene].ProcessInput(e);
}

void Menu::Update()
{
	mScenes[mScene].Update(mPos);
	//checks if a scenes button has been pressed and
	//decides what to do using the buttons text
	if (mScene == 2)
	{
		//updates the HUD
		std::pair<int, int> ammo = mLevel->GetPlayer()->GetGun()->GetAmmo();
		mHudTexts[0]->setString(std::to_string(ammo.first) + " / " + std::to_string(ammo.second));
		mHudRects[0]->first.setScale(mLevel->GetPlayer()->Health() / 10.f, 1);
		mHudRects[1]->first.setScale(mLevel->GetPlayer()->JetFuel() / 300.f, 1);
	}
}

void Menu::SetScene(int i)
{
	mScenes[mScene].SetVisible(false);
	mScene = i;
	mScenes[mScene].SetVisible(true);
	SetPaused(i != 2);
}

void Menu::SetPaused(bool b)
{
	mPaused = b;
	mWin->setMouseCursorVisible(mPaused);
}

bool Menu::GamePaused()
{
	return mPaused;
}

int Menu::GetScene()
{
	return mScene;
}