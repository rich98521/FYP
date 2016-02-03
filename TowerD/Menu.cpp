#include "stdafx.h"
#include "Menu.h"


void Menu::StartButtonClick()
{
	SetScene(1);
}

Menu::Menu(Renderer* ren, sf::Window* win, Level* l, Camera* c)
	:mLevel(l), mWin(win), mRen(ren), mCam(c)
{
	//sets up menu items (buttons, texts, scenes, rectangleshapes, panels)
	mScenes.push_back(Scene(ren, new Sprite("../Sprites/MainMenu.png")));
	Text* t = new Text("Robot Defense", "DISTGRG_.ttf");
	t->setCharacterSize(100);
	t->setPosition(200, 80);
	t->setColor(sf::Color(255, 0, 0, 255));
	mScenes.back().AddText(t);
	mScenes.back().AddButton(new Button(sf::FloatRect(520, 400, 160, 80), "Start", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(520, 490, 160, 80), "Options", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(520, 580, 160, 80), "Quit", ren));
	mScenes.push_back(Scene(ren, new Sprite("../Sprites/LevelSelect.png")));
	mScenes.back().AddButton(new Button(sf::FloatRect(800, 20, 120, 40), "Join Game", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(20, 20, 80, 40), "Back", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(4, 368, 192, 224), "Level 1", "../Sprites/Level1.png", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(204, 368, 192, 224), "Level 2", "../Sprites/Level2.png", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(404, 368, 192, 224), "Level 3", "../Sprites/Level3.png", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(604, 368, 192, 224), "Level 4", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(804, 368, 192, 224), "Level 5", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(1004, 368, 192, 224), "Level 6", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(4, 598, 192, 224), "Level 7", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(204, 598, 192, 224), "Level 8", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(404, 598, 192, 224), "Level 9", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(604, 598, 192, 224), "Level 10", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(804, 598, 192, 224), "Level 11", "../Sprites/Level4.png", ren));
	mScenes.back().AddButton(new Button(sf::FloatRect(1004, 598, 192, 224), "Level 12", "../Sprites/Level4.png", ren));
	mScenes.push_back(Scene(ren));
	mScenes.back().SetCanFocus(false);

	mScene = 2;
	SetScene(0);
}

void Menu::InitHud()
{
	mHudRects.clear();
	mHudTexts.clear();
	mScenes[2].ClearRects();
	mScenes[2].ClearPanels();
	mScenes[2].ClearTexts();
	mHudRects.push_back(new std::pair<sf::RectangleShape, bool>);
	mHudRects.back()->first = sf::RectangleShape(sf::Vector2f(148, 24));
	mHudRects.back()->first.setPosition(6, 842);
	mHudRects.back()->first.setFillColor(sf::Color(21, 249, 39, 200));
	mScenes[2].AddRect(mHudRects.back());
	mHudRects.push_back(new std::pair<sf::RectangleShape, bool>);
	mHudRects.back()->first = sf::RectangleShape(sf::Vector2f(148, 24));
	mHudRects.back()->first.setPosition(6, 870);
	mHudRects.back()->first.setFillColor(sf::Color(21, 39, 125, 200));
	mScenes[2].AddRect(mHudRects.back());
	mHudRects.push_back(new std::pair<sf::RectangleShape, bool>);
	mHudRects.back()->first = sf::RectangleShape(sf::Vector2f(46, 46));
	mHudRects.back()->first.setPosition(477, 852);
	mHudRects.back()->first.setOutlineColor(sf::Color(200, 0, 0, 200));
	mHudRects.back()->first.setOutlineThickness(2);
	mHudRects.back()->first.setFillColor(sf::Color(0, 0, 0, 0));
	mScenes[2].AddRect(mHudRects.back());
	mHudTexts.push_back(new Text("test", "detente.ttf"));
	mHudTexts.back()->setPosition(1100, 864);
	mHudTexts.back()->setColor(sf::Color(64, 64, 64, 240));
	mHudTexts.back()->setCharacterSize(15);
	mScenes[2].AddText(mHudTexts.back());
	mHudTexts.push_back(new Text("test", "detente.ttf"));
	mHudTexts.back()->setPosition(1000, 20);
	mHudTexts.back()->setColor(sf::Color(200, 150, 64, 240));
	mHudTexts.back()->setCharacterSize(30);
	mScenes[2].AddText(mHudTexts.back());
	Panel* p = new Panel(mRen, sf::FloatRect(sf::Vector2f(480, 320), sf::Vector2f(240, 370)));
	p->AddButton(new Button(sf::FloatRect(10, 10, 220, 80), "Resume", mRen));
	p->AddButton(new Button(sf::FloatRect(10, 100, 220, 80), "Options", mRen));
	p->AddButton(new Button(sf::FloatRect(10, 190, 220, 80), "Start Server", mRen));
	p->AddButton(new Button(sf::FloatRect(10, 280, 220, 80), "Quit", mRen));
	mScenes[2].AddPanel(p);
	Panel* p2 = new Panel(mRen, sf::FloatRect(sf::Vector2f(0, 320), sf::Vector2f(280, 370)));
	p2->AddButton(new Button(sf::FloatRect(10, 10, 260, 80), "Damage Lv. " + std::to_string(mLevel->GetPlayer()->GetDamageLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p2->AddButton(new Button(sf::FloatRect(10, 100, 260, 80), "Rate Lv. " + std::to_string(mLevel->GetPlayer()->GetRateLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p2->AddButton(new Button(sf::FloatRect(10, 190, 260, 80), "Accuracy Lv. " + std::to_string(mLevel->GetPlayer()->GetAccuracyLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p2->AddButton(new Button(sf::FloatRect(10, 280, 260, 80), "Ammo Lv. " + std::to_string(mLevel->GetPlayer()->GetAmmoLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	mScenes[2].AddPanel(p2);
	Panel* p3 = new Panel(mRen, sf::FloatRect(sf::Vector2f(0, 320), sf::Vector2f(280, 280)));
	p3->AddButton(new Button(sf::FloatRect(10, 10, 260, 80), "Health Lv. " + std::to_string(mLevel->GetPlayer()->GetHealthLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p3->AddButton(new Button(sf::FloatRect(10, 100, 260, 80), "Speed Lv. " + std::to_string(mLevel->GetPlayer()->GetSpeedLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p3->AddButton(new Button(sf::FloatRect(10, 190, 260, 80), "Fuel Lv. " + std::to_string(mLevel->GetPlayer()->GetFuelLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	mScenes[2].AddPanel(p3);
	Panel* p4 = new Panel(mRen, sf::FloatRect(sf::Vector2f(0, 220), sf::Vector2f(280, 100)));
	p4->AddButton(new Button(sf::FloatRect(10, 10, 125, 80), "Weapon", 30, mRen));
	p4->AddButton(new Button(sf::FloatRect(135, 10, 125, 80), "Suit", 30, mRen));
	mScenes[2].AddPanel(p4);
	Sprite* s = new Sprite("../Sprites/WaveUI.png");
	s->setPosition(sf::Vector2f(564, 0));
	Panel* p5 = new Panel(mRen, s);
	mHudTexts.push_back(new Text("1", "detente.ttf"));
	mHudTexts.back()->setPosition(sf::Vector2f(30, 5));
	mHudTexts.back()->setCharacterSize(24);
	mHudTexts.back()->setColor(sf::Color(200, 0, 0, 255));
	p5->AddText(mHudTexts.back());
	mScenes[2].AddPanel(p5);
	Sprite* s2 = new Sprite("../Sprites/HealthFuelUI.png");
	s2->setPosition(sf::Vector2f(1, 837));
	Panel* p6 = new Panel(mRen, s2);
	mScenes[2].AddPanel(p6);
	Sprite* s3 = new Sprite("../Sprites/AmmoUI.png");
	s3->setPosition(sf::Vector2f(1083, 837));
	Panel* p7 = new Panel(mRen, s3);
	mScenes[2].AddPanel(p7);
	Sprite* s4 = new Sprite("../Sprites/GunUI.png");
	s4->setPosition(sf::Vector2f(476, 851));
	Panel* p8 = new Panel(mRen, s4);
	mScenes[2].AddPanel(p8);
	Sprite* s5 = new Sprite("../Sprites/WallUI.png");
	s5->setPosition(sf::Vector2f(526, 851));
	Panel* p9 = new Panel(mRen, s5);
	mScenes[2].AddPanel(p9);
	Sprite* s6 = new Sprite("../Sprites/Turret1UI.png");
	s6->setPosition(sf::Vector2f(576, 851));
	Panel* p10 = new Panel(mRen, s6);
	mScenes[2].AddPanel(p10);
	Sprite* s7 = new Sprite("../Sprites/Turret2UI.png");
	s7->setPosition(sf::Vector2f(626, 851));
	Panel* p11 = new Panel(mRen, s7);
	mScenes[2].AddPanel(p11);
	Sprite* s8 = new Sprite("../Sprites/Turret3UI.png");
	s8->setPosition(sf::Vector2f(676, 851));
	Panel* p12 = new Panel(mRen, s8);
	mScenes[2].AddPanel(p12);
	Panel* p13 = new Panel(mRen, sf::FloatRect(sf::Vector2f(480, 320), sf::Vector2f(240, 190)));
	p13->SetCanFocus(true);
	Text* t = new Text("Core Destroyed", "");
	t->setPosition(sf::Vector2f(10, 10));
	t->setCharacterSize(20);
	p13->AddText(t);
	p13->AddButton(new Button(sf::FloatRect(10, 10, 220, 80), "Retry", mRen));
	p13->AddButton(new Button(sf::FloatRect(10, 100, 220, 80), "Quit", mRen));
	mScenes[2].AddPanel(p13);
	Panel* p14 = new Panel(mRen, sf::FloatRect(sf::Vector2f(480, 320), sf::Vector2f(240, 190)));
	p14->SetCanFocus(true);
	Text* t2 = new Text("You Win", "");
	t2->setPosition(sf::Vector2f(10, 10));
	t2->setCharacterSize(20);
	p14->AddText(t2);
	p14->AddButton(new Button(sf::FloatRect(10, 10, 220, 80), "Next Level", mRen));
	p14->AddButton(new Button(sf::FloatRect(10, 100, 220, 80), "Quit", mRen));
	mScenes[2].AddPanel(p14);
}

void Menu::UpdateTurretMenus()
{
	for (auto it = mTurretPanels.cbegin(); it != mTurretPanels.cend();)
	{
		if (std::find(mLevel->GetTurrets()->begin(), mLevel->GetTurrets()->end(), it->second) == mLevel->GetTurrets()->end())
		{
			auto i = std::find(mScenes[2].Panels()->begin(), mScenes[2].Panels()->end(), it->first);
			if (i != mScenes[2].Panels()->end())
			{
				mScenes[2].Panels()->erase(i);
				delete(it->first);
			}
			mTurretPanels.erase(it++);
		}
		else
			++it;
	}
	for each (Turret* t in *mLevel->GetTurrets())
	{
		Panel* p = 0;
		for each (std::pair<Panel*, Turret*> tp in mTurretPanels)
		{
			if (t == tp.second)
			{
				p = tp.first;
				break;
			}
		}

		if (p == 0)
		{
			p = new Panel(mRen, sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(70, 40)));
			mTurretPanels[p] = t;
			p->AddButton(new Button(sf::FloatRect(5, 5, 60, 30), "Lv. " + std::to_string(t->GetLevel() + 1) + " (" + std::to_string(t->GetUpgradeCost(t->GetLevel())) + "C)", mRen));
			p->SetVisible(mTurretPanelsVis);
			p->SetCanFocus(true);
			p->Buttons().back()->SetFontSize(12);
			mScenes[2].AddPanel(p);
		}
		p->SetPosition(sf::Vector2f((t->Location() - sf::Vector2f(18, 0) - mCam->Offset())*mCam->Scale()));
	}
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
					(*mScenes[mScene].Panels())[i]->SetEnabled(false);
				}
			}
			if (e.key.code == sf::Keyboard::Num1)
			{
				(*mScenes[mScene].Panels())[7]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(477, 852));
			}
			else if (e.key.code == sf::Keyboard::Num2)
			{
				(*mScenes[mScene].Panels())[8]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(527, 852));
			}
			else if (e.key.code == sf::Keyboard::Num3)
			{
				(*mScenes[mScene].Panels())[9]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(577, 852));
			}
			else if (e.key.code == sf::Keyboard::Num4)
			{
				(*mScenes[mScene].Panels())[10]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(627, 852));
			}
			else if (e.key.code == sf::Keyboard::Num5)
			{
				(*mScenes[mScene].Panels())[11]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(677, 852));
			}
		}
		if (e.type == sf::Event::KeyReleased)
		{
			if (e.key.code == sf::Keyboard::Escape)
			{
				//toggles pause menu panel
				(*mScenes[mScene].Panels())[0]->SetVisible(!(*mScenes[mScene].Panels())[0]->GetVisible());
				mPauseMenu = (*mScenes[mScene].Panels())[0]->GetVisible();
				mScenes[mScene].SetCanFocus((*mScenes[mScene].Panels())[0]->GetVisible());
			}
			else if (e.key.code == sf::Keyboard::Tab)
			{
				//toggles Upgrades panel
				bool upgradeVisible = (*mScenes[mScene].Panels())[3]->GetVisible();
				(*mScenes[mScene].Panels())[3]->SetVisible(!upgradeVisible);
				if (!upgradeVisible)
					(*mScenes[mScene].Panels())[1]->SetVisible(true);
				else
				{
					(*mScenes[mScene].Panels())[1]->SetVisible(false);
					(*mScenes[mScene].Panels())[2]->SetVisible(false);
				}
			}
			else if (e.key.code == sf::Keyboard::Q)
			{
				//toggles Turret Upgrade Panels
				mTurretPanelsVis = !mTurretPanelsVis;
				for (auto it = mTurretPanels.cbegin(); it != mTurretPanels.cend(); it++)
					it->first->SetVisible(mTurretPanelsVis);
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
				mLevel->LoadLevel(std::atoi(text.substr(6).c_str()));
				SetScene(2);
			}
			else if (text == "Join Game")
			{
			}
		}
		else if (mScene == 2)
		{
			if (text == "Resume")
			{
				(*mScenes[mScene].Panels())[0]->SetVisible(!(*mScenes[mScene].Panels())[0]->GetVisible());
				SetPaused((*mScenes[mScene].Panels())[0]->GetVisible());
				mScenes[mScene].SetCanFocus((*mScenes[mScene].Panels())[0]->GetVisible());
			}
			else if (text == "Options")
			{
			}
			else if (text == "Start Server")
			{
			}
			else if (text == "Retry")
			{
				mLevel->RestartCurrentWave();
			}
			else if (text == "Quit")
			{
				//mWin->close();
				mScenes[mScene].SetCanFocus(false);
				SetScene(0);
			}
			else if (text == "Next Level")
			{
				mLevel->LoadNextLevel();
			}
			else if (text == "Weapon")
			{
				(*mScenes[mScene].Panels())[1]->SetVisible(true);
				(*mScenes[mScene].Panels())[2]->SetVisible(false);
			}
			else if (text == "Suit")
			{
				(*mScenes[mScene].Panels())[2]->SetVisible(true);
				(*mScenes[mScene].Panels())[1]->SetVisible(false);
			}
			else if (text.substr(0, 6) == "Damage")
			{
				int level = mLevel->GetPlayer()->GetDamageLevel();
				if (level < 4){
					int upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level);
					if (mLevel->GetPlayer()->GetCredits() >= upgradeCost){
						mLevel->GetPlayer()->AddCredits(-upgradeCost);
						mLevel->GetPlayer()->SetDamageLevel(level + 1);
						upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level + 1);
						b->SetText("Damage Lv. " + std::to_string(level + 2) + (level + 1 < 4 ? "\n(" + std::to_string(upgradeCost) + "C)" : "\nMAX"));
					}
				}
			}
			else if (text.substr(0, 4) == "Rate")
			{
				int level = mLevel->GetPlayer()->GetRateLevel();
				if (level < 4){
					int upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level);
					if (mLevel->GetPlayer()->GetCredits() >= upgradeCost){
						mLevel->GetPlayer()->AddCredits(-upgradeCost);
						mLevel->GetPlayer()->SetRateLevel(level + 1);
						upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level + 1);
						b->SetText("Rate Lv. " + std::to_string(level + 2) + (level + 1 < 4 ? "\n(" + std::to_string(upgradeCost) + "C)" : "\nMAX"));
					}
				}
			}
			else if (text.substr(0, 8) == "Accuracy")
			{
				int level = mLevel->GetPlayer()->GetAccuracyLevel();
				if (level < 4){
					int upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level);
					if (mLevel->GetPlayer()->GetCredits() >= upgradeCost){
						mLevel->GetPlayer()->AddCredits(-upgradeCost);
						mLevel->GetPlayer()->SetAccuracyLevel(level + 1);
						upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level + 1);
						b->SetText("Accuracy Lv. " + std::to_string(level + 2) + (level + 1 < 4 ? "\n(" + std::to_string(upgradeCost) + "C)" : "\nMAX"));
					}
				}
			}
			else if (text.substr(0, 4) == "Ammo")
			{
				int level = mLevel->GetPlayer()->GetAmmoLevel();
				if (level < 4){
					int upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level);
					if (mLevel->GetPlayer()->GetCredits() >= upgradeCost){
						mLevel->GetPlayer()->AddCredits(-upgradeCost);
						mLevel->GetPlayer()->SetAmmoLevel(level + 1);
						upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level + 1);
						b->SetText("Ammo Lv. " + std::to_string(level + 2) + (level + 1 < 4 ? "\n(" + std::to_string(upgradeCost) + "C)" : "\nMAX"));
					}
				}
			}
			else if (text.substr(0, 6) == "Health")
			{
				int level = mLevel->GetPlayer()->GetHealthLevel();
				if (level < 4){
					int upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level);
					if (mLevel->GetPlayer()->GetCredits() >= upgradeCost){
						mLevel->GetPlayer()->AddCredits(-upgradeCost);
						mLevel->GetPlayer()->SetHealthLevel(level + 1);
						upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level + 1);
						b->SetText("Health Lv. " + std::to_string(level + 2) + (level + 1 < 4 ? "\n(" + std::to_string(upgradeCost) + "C)" : "\nMAX"));
					}
				}
			}
			else if (text.substr(0, 5) == "Speed")
			{
				int level = mLevel->GetPlayer()->GetSpeedLevel();
				if (level < 4){
					int upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level);
					if (mLevel->GetPlayer()->GetCredits() >= upgradeCost){
						mLevel->GetPlayer()->AddCredits(-upgradeCost);
						mLevel->GetPlayer()->SetSpeedLevel(level + 1);
						upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level + 1);
						b->SetText("Speed Lv. " + std::to_string(level + 2) + (level + 1 < 4 ? "\n(" + std::to_string(upgradeCost) + "C)" : "\nMAX"));
					}
				}
			}
			else if (text.substr(0, 4) == "Fuel")
			{
				int level = mLevel->GetPlayer()->GetFuelLevel();
				if (level < 4){
					int upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level);
					if (mLevel->GetPlayer()->GetCredits() >= upgradeCost){
						mLevel->GetPlayer()->AddCredits(-upgradeCost);
						mLevel->GetPlayer()->SetFuelLevel(level + 1);
						upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level + 1);
						b->SetText("Fuel Lv. " + std::to_string(level + 2) + (level + 1 < 4 ? "\n(" + std::to_string(upgradeCost) + "C)" : "\nMAX"));
					}
				}
			}
			else if (text.substr(0, 3) == "Lv.")
			{
				Turret* t = mTurretPanels[b->GetPanel()];
				int level = t->GetLevel();
				if (level < 4){
					int upgradeCost = t->GetUpgradeCost(level);
					if (mLevel->GetPlayer()->GetCredits() >= upgradeCost){
						mLevel->GetPlayer()->AddCredits(-upgradeCost);
						t->SetLevel(level + 1);
						upgradeCost = t->GetUpgradeCost(level + 1);
						b->SetText("Lv. " + std::to_string(level + 2) + (level + 1 < 4 ? " (" + std::to_string(upgradeCost) + "C)" : " MAX"));
					}
				}
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
		mHudTexts[2]->setString(std::to_string(mLevel->GetWave()));
		mHudRects[0]->first.setScale(mLevel->GetPlayer()->Health() / mLevel->GetPlayer()->GetMaxHealth(), 1);
		mHudRects[1]->first.setScale(mLevel->GetPlayer()->JetFuel() / mLevel->GetPlayer()->GetMaxFuel(), 1);
		UpdateTurretMenus();
		(*mScenes[2].Panels())[12]->SetVisible(mLevel->GameOver());
		(*mScenes[2].Panels())[13]->SetVisible(mLevel->Won());
		SetPaused(mLevel->GameOver() || mLevel->Won() || mPauseMenu);
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
	if (i == 2)
	{
		InitHud();
		for (int i = 4; i < (*mScenes[mScene].Panels()).size(); i++)
		{
			(*mScenes[mScene].Panels())[i]->SetVisible(true);
		}
		mTurretPanelsVis = false;
		mPauseMenu = false;
	}
	mScenes[mScene].SetVisible(true);
	SetPaused(i != 2);
}

void Menu::SetPaused(bool b)
{
	if (mPaused != b){
		mPaused = b;
		mWin->setMouseCursorVisible(mPaused);
	}
}

bool Menu::GamePaused()
{
	return mPaused;
}

int Menu::GetScene()
{
	return mScene;
}