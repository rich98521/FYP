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
	mScenes.push_back(Scene(ren));
	mScenes.back().SetCanFocus(false);
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
	mHudRects.push_back(new std::pair<sf::RectangleShape, bool>);
	mHudRects.back()->first = sf::RectangleShape(sf::Vector2f(46, 46));
	mHudRects.back()->first.setPosition(477, 852);
	mHudRects.back()->first.setOutlineColor(sf::Color(200, 0, 0, 200));
	mHudRects.back()->first.setOutlineThickness(2);
	mHudRects.back()->first.setFillColor(sf::Color(0, 0, 0, 0));
	mScenes.back().AddRect(mHudRects.back());
	mHudTexts.push_back(new Text("test", "detente.ttf"));
	mHudTexts.back()->setPosition(1100, 864);
	mHudTexts.back()->setColor(sf::Color(64, 64, 64, 240));
	mHudTexts.back()->setCharacterSize(15);
	mScenes.back().AddText(mHudTexts.back());
	mHudTexts.push_back(new Text("test", "detente.ttf"));
	mHudTexts.back()->setPosition(1100, 20);
	mHudTexts.back()->setColor(sf::Color(64, 64, 64, 240));
	mHudTexts.back()->setCharacterSize(15);
	mScenes.back().AddText(mHudTexts.back());
	Panel* p = new Panel(ren, sf::IntRect(sf::Vector2i(480, 320), sf::Vector2i(240, 370)));
	p->AddButton(new Button(sf::IntRect(10, 10, 220, 80), "Resume", ren));
	p->AddButton(new Button(sf::IntRect(10, 100, 220, 80), "Options", ren));
	p->AddButton(new Button(sf::IntRect(10, 190, 220, 80), "Start Server", ren));
	p->AddButton(new Button(sf::IntRect(10, 280, 220, 80), "Quit", ren));
	mScenes.back().AddPanel(p);
	Panel* p2 = new Panel(ren, sf::IntRect(sf::Vector2i(0, 320), sf::Vector2i(280, 370)));
	p2->AddButton(new Button(sf::IntRect(10, 10, 260, 80), "Caliber Lv. 1\n   (200C)", 20, ren));
	p2->AddButton(new Button(sf::IntRect(10, 100, 260, 80), "Rate Lv. 1\n (200C)", 20, ren));
	p2->AddButton(new Button(sf::IntRect(10, 190, 260, 80), "Spread Lv. 1\n   (200C)", 20, ren));
	p2->AddButton(new Button(sf::IntRect(10, 280, 260, 80), "Ammo Lv. 1\n   (200C)", 20, ren));
	mScenes.back().AddPanel(p2);
	Panel* p3 = new Panel(ren, sf::IntRect(sf::Vector2i(0, 320), sf::Vector2i(280, 280)));
	p3->AddButton(new Button(sf::IntRect(10, 10, 260, 80), "Armour Lv. 1\n   (200C)", 20, ren));
	p3->AddButton(new Button(sf::IntRect(10, 100, 260, 80), "Speed Lv. 1\n  (200C)", 20, ren));
	p3->AddButton(new Button(sf::IntRect(10, 190, 260, 80), "Fuel Lv. 1\n (200C)", 20, ren));
	mScenes.back().AddPanel(p3);
	Panel* p4 = new Panel(ren, sf::IntRect(sf::Vector2i(0, 220), sf::Vector2i(280, 100)));
	p4->AddButton(new Button(sf::IntRect(10, 10, 125, 80), "Weapon", 30, ren));
	p4->AddButton(new Button(sf::IntRect(135, 10, 125, 80), "Suit", 30, ren));
	mScenes.back().AddPanel(p4);
	Sprite* s = new Sprite("../Sprites/WaveUI.png");
	s->setPosition(sf::Vector2f(564, 0));
	Panel* p5 = new Panel(ren, s);
	mScenes.back().AddPanel(p5);
	Sprite* s2 = new Sprite("../Sprites/HealthFuelUI.png");
	s2->setPosition(sf::Vector2f(1, 837));
	Panel* p6 = new Panel(ren, s2);
	mScenes.back().AddPanel(p6);
	Sprite* s3 = new Sprite("../Sprites/AmmoUI.png");
	s3->setPosition(sf::Vector2f(1083, 837));
	Panel* p7 = new Panel(ren, s3);
	mScenes.back().AddPanel(p7);
	Sprite* s4 = new Sprite("../Sprites/GunUI.png");
	s4->setPosition(sf::Vector2f(476, 851));
	Panel* p8 = new Panel(ren, s4);
	mScenes.back().AddPanel(p8);
	Sprite* s5 = new Sprite("../Sprites/WallUI.png");
	s5->setPosition(sf::Vector2f(526, 851));
	Panel* p9 = new Panel(ren, s5);
	mScenes.back().AddPanel(p9);
	Sprite* s6 = new Sprite("../Sprites/Turret1UI.png");
	s6->setPosition(sf::Vector2f(576, 851));
	Panel* p10 = new Panel(ren, s6);
	mScenes.back().AddPanel(p10);
	Sprite* s7 = new Sprite("../Sprites/Turret2UI.png");
	s7->setPosition(sf::Vector2f(626, 851));
	Panel* p11 = new Panel(ren, s7);
	mScenes.back().AddPanel(p11);
	Sprite* s8 = new Sprite("../Sprites/Turret3UI.png");
	s8->setPosition(sf::Vector2f(676, 851));
	Panel* p12 = new Panel(ren, s8);
	mScenes.back().AddPanel(p12);
	

	mScene = 2;
	SetScene(0);
}

bool Menu::ProcessInput(sf::Event e)
{
	mFocus = false;
	if (e.type == sf::Event::MouseMoved)
	{
		mPos = sf::Vector2i(e.mouseMove.x, e.mouseMove.y);
	}
	if (mScene == 2)
	{
		if (e.type == sf::Event::KeyPressed)
		{
			if (e.key.code == sf::Keyboard::Num1 || e.key.code == sf::Keyboard::Num2 || e.key.code == sf::Keyboard::Num3 || e.key.code == sf::Keyboard::Num4 || e.key.code == sf::Keyboard::Num5)
			{
				for (int i = 7; i < 12; i++)
				{
					mScenes[mScene].Panels()[i]->SetEnabled(false);
				}
			}
			if (e.key.code == sf::Keyboard::Num1)
			{
				mScenes[mScene].Panels()[7]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(477, 852));
			}
			else if (e.key.code == sf::Keyboard::Num2)
			{
				mScenes[mScene].Panels()[8]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(527, 852));
			}
			else if (e.key.code == sf::Keyboard::Num3)
			{
				mScenes[mScene].Panels()[9]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(577, 852));
			}
			else if (e.key.code == sf::Keyboard::Num4)
			{
				mScenes[mScene].Panels()[10]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(627, 852));
			}
			else if (e.key.code == sf::Keyboard::Num5)
			{
				mScenes[mScene].Panels()[11]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(677, 852));
			}
		}
		if (e.type == sf::Event::KeyReleased)
		{
			if (e.key.code == sf::Keyboard::Escape)
			{
				//toggles pause menu panel
				mScenes[mScene].Panels()[0]->SetVisible(!mScenes[mScene].Panels()[0]->GetVisible());
				SetPaused(mScenes[mScene].Panels()[0]->GetVisible());
				mScenes[mScene].SetCanFocus(mScenes[mScene].Panels()[0]->GetVisible());
			}
			else if (e.key.code == sf::Keyboard::Tab)
			{
				//toggles Upgrades panel
				bool upgradeVisible = mScenes[mScene].Panels()[3]->GetVisible();
				mScenes[mScene].Panels()[3]->SetVisible(!upgradeVisible);
				if (!upgradeVisible)
					mScenes[mScene].Panels()[1]->SetVisible(true);
				else
				{
					mScenes[mScene].Panels()[1]->SetVisible(false);
					mScenes[mScene].Panels()[2]->SetVisible(false);
				}
			}
		}
	}
	mFocus = mScenes[mScene].ProcessInput(e);
	mWin->setMouseCursorVisible(mFocus);
	return mFocus;
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
				mScenes[mScene].SetCanFocus(mScenes[mScene].Panels()[0]->GetVisible());
			}
			else if (text == "Options")
			{
			}
			else if (text == "Start Server")
			{
			}
			else if (text == "Quit")
			{
				//mWin->close();
				mScenes[mScene].SetCanFocus(false);
				SetScene(0);
			}
			else if (text == "Weapon")
			{
				mScenes[mScene].Panels()[1]->SetVisible(true);
				mScenes[mScene].Panels()[2]->SetVisible(false);
			}
			else if (text == "Suit")
			{
				mScenes[mScene].Panels()[2]->SetVisible(true);
				mScenes[mScene].Panels()[1]->SetVisible(false);
			}
			else if (text.substr(0,7) == "Caliber")
			{

			}
			else if (text.substr(0, 4) == "Rate")
			{

			}
			else if (text.substr(0, 5) == "Spread")
			{

			}
			else if (text.substr(0, 4) == "Ammo")
			{

			}
			else if (text.substr(0, 5) == "Armour")
			{

			}
			else if (text.substr(0, 5) == "Speed")
			{

			}
			else if (text.substr(0, 4) == "Fuel")
			{

			}
		}
		//window.setMouseCursorVisible(false);
	}
	if (mScene == 2)
	{
		//updates the HUD
		std::pair<int, int> ammo = mLevel->GetPlayer()->GetGun()->GetAmmo();
		mHudTexts[0]->setString(std::to_string(ammo.first) + " / " + std::to_string(ammo.second));
		mHudTexts[1]->setString("Credits: " + std::to_string(mLevel->GetPlayer()->GetCredits()));
		mHudRects[0]->first.setScale(mLevel->GetPlayer()->Health() / 10.f, 1);
		mHudRects[1]->first.setScale(mLevel->GetPlayer()->JetFuel() / 300.f, 1);
	}
}

void Menu::SetScene(int i)
{
	if (mScene == 2 && i == 0)
	{
		SoundManager::PlayMusic("Menu");
	}
	mScenes[mScene].SetVisible(false);
	mScene = i;
	mScenes[mScene].SetVisible(true);
	if (i == 2)
	{
		for (int i = 4; i < mScenes[mScene].Panels().size(); i++)
		{
			mScenes[mScene].Panels()[i]->SetVisible(true);
		}
	}
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