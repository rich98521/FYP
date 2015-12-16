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
	mScenes.back().AddButton(new Button(sf::IntRect(520, 400, 160, 80), "Start", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(520, 490, 160, 80), "Options", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(520, 580, 160, 80), "Quit", ren));
	mScenes.push_back(Scene(ren, new Sprite("../Sprites/LevelSelect.png")));
	mScenes.back().AddButton(new Button(sf::IntRect(800, 20, 120, 40), "Join Game", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(20, 20, 80, 40), "Back", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(4, 368, 192, 224), "Level 1", "../Sprites/Level1.png", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(204, 368, 192, 224), "Level 2", "../Sprites/Level2.png", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(404, 368, 192, 224), "Level 3", "../Sprites/Level3.png", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(604, 368, 192, 224), "Level 4", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(804, 368, 192, 224), "Level 5", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(1004, 368, 192, 224), "Level 6", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(4, 598, 192, 224), "Level 7", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(204, 598, 192, 224), "Level 8", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(404, 598, 192, 224), "Level 9", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(604, 598, 192, 224), "Level 10", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(804, 598, 192, 224), "Level 11", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::IntRect(1004, 598, 192, 224), "Level 12", "../Sprites/Level4.png", ren));
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
	Panel* p = new Panel(ren, sf::IntRect(sf::Vector2i(480, 320), sf::Vector2i(240, 370)));
	p->AddButton(new Button(sf::IntRect(10, 10, 220, 80), "Resume", ren));
	p->AddButton(new Button(sf::IntRect(10, 100, 220, 80), "Options", ren));
	p->AddButton(new Button(sf::IntRect(10, 190, 220, 80), "Start Server", ren));
	p->AddButton(new Button(sf::IntRect(10, 280, 220, 80), "Quit", ren));
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
	Button* b;
	if ((b = mScenes[mScene].DownButton()) != 0)
	{
		string text = b->GetText();
		if (mScene == 0)
		{
			if (text == "Start")
			{
				SetScene(1);
			}
			else if (text == "Options")
			{
			}
			else if (text == "Quit")
			{
				mWin->close();
			}
		}
		else if (mScene == 1)
		{
			if (text == "Back")
			{
				SetScene(0);
			}
			else if (text.substr(0, 5) == "Level")
			{
				SetScene(2);
				mLevel->LoadLevel(std::atoi(text.substr(6).c_str()));
			}
			else if (text == "Join Game")
			{
			}
		}
		else if (mScene == 2)
		{
			if (text == "Resume")
			{
				mScenes[mScene].Panels()[0]->SetVisible(!mScenes[mScene].Panels()[0]->GetVisible());
				SetPaused(mScenes[mScene].Panels()[0]->GetVisible());
			}
			else if (text == "Options")
			{
			}
			else if (text == "Start Server")
			{
			}
			else if (text == "Quit")
			{
				mWin->close();
			}
		}
		//window.setMouseCursorVisible(false);
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