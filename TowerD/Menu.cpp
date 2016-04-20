#include "stdafx.h"
#include "Menu.h"

Menu::Menu(Renderer* ren, sf::Window* win, Level* l, Camera* c)
	:mLevel(l), mWin(win), mRen(ren), mCam(c)
{
	//sets up menu items (buttons, texts, scenes, rectangleshapes, panels)
	mScenes.push_back(new Scene(ren, new Sprite("../Sprites/MainMenu.png")));
	Text* t = new Text("Robot Defense", "DISTGRG_.ttf");
	mSelectedTurret = 0;
	t->setCharacterSize(100);
	t->setPosition(200, 80);
	t->setColor(sf::Color(255, 0, 0, 255));
	mScenes.back()->AddText(t);
	mScenes.back()->AddButton(new Button(sf::FloatRect(520, 400, 160, 80), "Start", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(-1, 2, -1, 1);
	mScenes.back()->AddButton(new Button(sf::FloatRect(520, 490, 160, 80), "Options", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(-1, 0, -1, 2);
	mScenes.back()->AddButton(new Button(sf::FloatRect(520, 580, 160, 80), "Quit", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(-1, 1, -1, 0);
	mScenes.push_back(new Scene(ren, true));
	mScenes.back()->SetBackColor(sf::Color(58, 58, 58, 255));
	//mScenes.back()->AddButton(new Button(sf::FloatRect(800, 20, 120, 40), "Join Game", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(1, 13, 1, 2);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(20, 20, 80, 40), "Back", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(0, 13, 0, 2);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(4, 368, 192, 224), "Level 1", "../Sprites/Level1.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(7, 1, 3, 8);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(204, 368, 192, 224), "Level 2", "../Sprites/Level2.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(2, 1, 4, 9);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(404, 368, 192, 224), "Level 3", "../Sprites/Level3.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(3, 1, 5, 10);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(604, 368, 192, 224), "Level 4", "../Sprites/Level4.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(4, 0, 6, 11);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(804, 368, 192, 224), "Level 5", "../Sprites/Level4.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(5, 0, 7, 12);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(1004, 368, 192, 224), "Level 6", "../Sprites/Level4.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(6, 0, 2, 13);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(4, 598, 192, 224), "Level 7", "../Sprites/Level4.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(13, 2, 9, 1);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(204, 598, 192, 224), "Level 8", "../Sprites/Level4.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(8, 3, 10, 1);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(404, 598, 192, 224), "Level 9", "../Sprites/Level4.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(9, 4, 11, 1);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(604, 598, 192, 224), "Level 10", "../Sprites/Level4.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(10, 5, 12, 0);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(804, 598, 192, 224), "Level 11", "../Sprites/Level4.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(11, 6, 13, 0);
	//mScenes.back()->AddButton(new Button(sf::FloatRect(1004, 598, 192, 224), "Level 12", "../Sprites/Level4.png", ren));
	//mScenes.back()->GetButtons()->back()->SetAdjacent(12, 7, 8, 0);
	mScenes.back()->AddButton(new Button(sf::FloatRect(800, 20, 120, 40), "Join Game", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(1, 7, 1, 2);
	mScenes.back()->AddButton(new Button(sf::FloatRect(20, 20, 80, 40), "Back", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(0, 7, 0, 2);
	mScenes.back()->AddButton(new Button(sf::FloatRect(4, 368, 192, 224), "Level 1", "../Sprites/Level1.png", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(7, 1, 3, 1);
	mScenes.back()->AddButton(new Button(sf::FloatRect(204, 368, 192, 224), "Level 2", "../Sprites/Level2.png", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(2, 1, 4, 1);
	mScenes.back()->AddButton(new Button(sf::FloatRect(404, 368, 192, 224), "Level 3", "../Sprites/Level3.png", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(3, 1, 5, 1);
	mScenes.back()->AddButton(new Button(sf::FloatRect(604, 368, 192, 224), "Level 4", "../Sprites/Level4.png", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(4, 0, 6, 0);
	mScenes.back()->AddButton(new Button(sf::FloatRect(804, 368, 192, 224), "Level 5", "../Sprites/Level5.png", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(5, 0, 7, 0);
	mScenes.back()->AddButton(new Button(sf::FloatRect(1004, 368, 192, 224), "Level 6", "../Sprites/Level6.png", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(6, 0, 2, 0);
	mScenes.push_back(new Scene(ren, false));
	mScenes.back()->SetCanFocus(false);
	mScenes.push_back(new Scene(ren, true));
	mScenes.back()->SetBackColor(sf::Color(58, 58, 58, 255));
	mScenes.back()->AddSlider(new Slider(sf::FloatRect(200, 80, 200, 40), "Music Volume", mRen, 100));
	mScenes.back()->AddSlider(new Slider(sf::FloatRect(200, 140, 200, 40), "Effect Volume", mRen, 100));
	mScenes.back()->AddButton(new Button(sf::FloatRect(20, 20, 80, 40), "Back", ren));
	mScenes.back()->AddTextBox(new TextBox(sf::FloatRect(200, 200, 537, 40), "Multiplayer Name", "Player", mRen, 24));
	mScenes.push_back(new Scene(ren, true));
	mScenes.back()->SetBackColor(sf::Color(58, 58, 58, 255));
	mScenes.back()->AddButton(new Button(sf::FloatRect(20, 20, 80, 40), "Back", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(1, 1, 1, 1);
	mScenes.back()->AddButton(new Button(sf::FloatRect(800, 400, 80, 40), "Refresh", ren));
	mScenes.back()->GetButtons()->back()->SetAdjacent(0, 0, 0, 0);

	mScene = GAME;
	SetScene(MAIN);
}

void Menu::InitHud()
{
	mHudRects.clear();
	mHudTexts.clear();
	mScenes[GAME]->ClearRects();
	mScenes[GAME]->ClearPanels();
	mScenes[GAME]->ClearTexts();
	mHudRects.push_back(new std::pair<sf::RectangleShape, bool>);
	mHudRects.back()->first = sf::RectangleShape(sf::Vector2f(148, 24));
	mHudRects.back()->first.setPosition(6, 842);
	mHudRects.back()->first.setFillColor(sf::Color(21, 249, 39, 200));
	mScenes[GAME]->AddRect(mHudRects.back());
	mHudRects.push_back(new std::pair<sf::RectangleShape, bool>);
	mHudRects.back()->first = sf::RectangleShape(sf::Vector2f(148, 24));
	mHudRects.back()->first.setPosition(6, 870);
	mHudRects.back()->first.setFillColor(sf::Color(21, 39, 125, 200));
	mScenes[GAME]->AddRect(mHudRects.back());
	mHudRects.push_back(new std::pair<sf::RectangleShape, bool>);
	mHudRects.back()->first = sf::RectangleShape(sf::Vector2f(46, 46));
	mHudRects.back()->first.setPosition(477, 852);
	mHudRects.back()->first.setOutlineColor(sf::Color(200, 0, 0, 200));
	mHudRects.back()->first.setOutlineThickness(2);
	mHudRects.back()->first.setFillColor(sf::Color(0, 0, 0, 0));
	mScenes[GAME]->AddRect(mHudRects.back());
	mHudTexts.push_back(new Text("test", "detente.ttf"));
	mHudTexts.back()->setPosition(1090, 858);
	mHudTexts.back()->setColor(sf::Color(64, 64, 64, 240));
	mHudTexts.back()->setCharacterSize(18);
	mScenes[GAME]->AddText(mHudTexts.back());
	mHudTexts.push_back(new Text("test", "detente.ttf"));
	mHudTexts.back()->setPosition(1000, 20);
	mHudTexts.back()->setColor(sf::Color(200, 150, 64, 240));
	mHudTexts.back()->setCharacterSize(30);
	mScenes[GAME]->AddText(mHudTexts.back());
	Panel* p = new Panel(mRen, sf::FloatRect(sf::Vector2f(480, 320), sf::Vector2f(240, 370)));
	p->AddButton(new Button(sf::FloatRect(10, 10, 220, 80), "Resume", mRen));
	p->Buttons()->back()->SetAdjacent(-1, 3, -1, 1);
	p->AddButton(new Button(sf::FloatRect(10, 100, 220, 80), "Options", mRen));
	p->Buttons()->back()->SetAdjacent(-1, 0, -1, 2);
	p->AddButton(new Button(sf::FloatRect(10, 190, 220, 80), "Start Server", mRen));
	p->Buttons()->back()->SetAdjacent(-1, 1, -1, 3);
	p->AddButton(new Button(sf::FloatRect(10, 280, 220, 80), "Quit", mRen));
	p->Buttons()->back()->SetAdjacent(-1, 2, -1, 0);
	mScenes[GAME]->AddPanel(p);
	Panel* p2 = new Panel(mRen, sf::FloatRect(sf::Vector2f(0, 320), sf::Vector2f(280, 370)));
	p2->AddButton(new Button(sf::FloatRect(10, 10, 260, 80), "Damage Lv. " + std::to_string(mLevel->GetPlayer()->GetDamageLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p2->Buttons()->back()->SetAdjacent(-1, 12, -1, 5);
	p2->AddButton(new Button(sf::FloatRect(10, 100, 260, 80), "Rate Lv. " + std::to_string(mLevel->GetPlayer()->GetRateLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p2->Buttons()->back()->SetAdjacent(-1, 4, -1, 6);
	p2->AddButton(new Button(sf::FloatRect(10, 190, 260, 80), "Accuracy Lv. " + std::to_string(mLevel->GetPlayer()->GetAccuracyLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p2->Buttons()->back()->SetAdjacent(-1, 5, -1, 7);
	p2->AddButton(new Button(sf::FloatRect(10, 280, 260, 80), "Ammo Lv. " + std::to_string(mLevel->GetPlayer()->GetAmmoLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p2->Buttons()->back()->SetAdjacent(-1, 6, -1, 12);
	mScenes[GAME]->AddPanel(p2);
	Panel* p3 = new Panel(mRen, sf::FloatRect(sf::Vector2f(0, 320), sf::Vector2f(280, 370)));
	p3->AddButton(new Button(sf::FloatRect(10, 10, 260, 80), "Health Lv. " + std::to_string(mLevel->GetPlayer()->GetHealthLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p3->Buttons()->back()->SetAdjacent(-1, 13, -1, 9);
	p3->AddButton(new Button(sf::FloatRect(10, 100, 260, 80), "Speed Lv. " + std::to_string(mLevel->GetPlayer()->GetSpeedLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p3->Buttons()->back()->SetAdjacent(-1, 8, -1, 10);
	p3->AddButton(new Button(sf::FloatRect(10, 190, 260, 80), "Fuel Lv. " + std::to_string(mLevel->GetPlayer()->GetFuelLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p3->Buttons()->back()->SetAdjacent(-1, 9, -1, 11);
	p3->AddButton(new Button(sf::FloatRect(10, 280, 260, 80), "Grenade Lv. " + std::to_string(mLevel->GetPlayer()->GetPouchLevel() + 1) + "\n(" + std::to_string(mLevel->GetPlayer()->GetUpgradeCost(0)) + "C)", 20, mRen));
	p3->Buttons()->back()->SetAdjacent(-1, 10, -1, 13);
	mScenes[GAME]->AddPanel(p3);
	Panel* p4 = new Panel(mRen, sf::FloatRect(sf::Vector2f(0, 220), sf::Vector2f(280, 100)));
	p4->AddButton(new Button(sf::FloatRect(10, 10, 130, 80), "Weapon", 30, mRen));
	p4->Buttons()->back()->SetAdjacent(13, 7, 13, 4);
	p4->AddButton(new Button(sf::FloatRect(140, 10, 125, 80), "Suit", 30, mRen));
	p4->Buttons()->back()->SetAdjacent(12, 11, 12, 8);
	mScenes[GAME]->AddPanel(p4);
	Sprite* s = new Sprite("../Sprites/WaveUI.png");
	s->setPosition(sf::Vector2f(564, 0));
	Panel* p5 = new Panel(mRen, s);
	mHudTexts.push_back(new Text("1", "detente.ttf"));
	mHudTexts.back()->setPosition(sf::Vector2f(30, 5));
	mHudTexts.back()->setCharacterSize(24);
	mHudTexts.back()->setColor(sf::Color(200, 0, 0, 255));
	p5->AddText(mHudTexts.back());
	mScenes[GAME]->AddPanel(p5);
	mHudTexts.push_back(new Text("test", "detente.ttf"));
	mHudTexts.back()->setPosition(1170, 858);
	mHudTexts.back()->setColor(sf::Color(64, 64, 64, 240));
	mHudTexts.back()->setCharacterSize(18);
	mScenes[GAME]->AddText(mHudTexts.back());
	Sprite* s2 = new Sprite("../Sprites/HealthFuelUI.png");
	s2->setPosition(sf::Vector2f(1, 837));
	Panel* p6 = new Panel(mRen, s2);
	mScenes[GAME]->AddPanel(p6);
	Sprite* s3 = new Sprite("../Sprites/AmmoUI.png");
	s3->setPosition(sf::Vector2f(1083, 837));
	Panel* p7 = new Panel(mRen, s3);
	mScenes[GAME]->AddPanel(p7);
	Sprite* s4 = new Sprite("../Sprites/GunUI.png");
	s4->setPosition(sf::Vector2f(476, 851));
	Panel* p8 = new Panel(mRen, s4);
	mScenes[GAME]->AddPanel(p8);
	Sprite* s5 = new Sprite("../Sprites/WallUI.png");
	s5->setPosition(sf::Vector2f(526, 851));
	Panel* p9 = new Panel(mRen, s5);
	mScenes[GAME]->AddPanel(p9);
	Sprite* s6 = new Sprite("../Sprites/Turret1UI.png");
	s6->setPosition(sf::Vector2f(576, 851));
	Panel* p10 = new Panel(mRen, s6);
	mScenes[GAME]->AddPanel(p10);
	Sprite* s7 = new Sprite("../Sprites/Turret2UI.png");
	s7->setPosition(sf::Vector2f(626, 851));
	Panel* p11 = new Panel(mRen, s7);
	mScenes[GAME]->AddPanel(p11);
	Sprite* s8 = new Sprite("../Sprites/Turret3UI.png");
	s8->setPosition(sf::Vector2f(676, 851));
	Panel* p12 = new Panel(mRen, s8);
	mScenes[GAME]->AddPanel(p12);
	Panel* p13 = new Panel(mRen, sf::FloatRect(sf::Vector2f(480, 320), sf::Vector2f(240, 190)));
	p13->SetCanFocus(true);
	Text* t = new Text("Core Destroyed", "detente.ttf");
	t->setPosition(sf::Vector2f(10, 10));
	t->setCharacterSize(50);
	p13->AddText(t);
	p13->AddButton(new Button(sf::FloatRect(10, 10, 220, 80), "Retry", mRen));
	p13->Buttons()->back()->SetAdjacent(-1, 15, -1, 15);
	p13->AddButton(new Button(sf::FloatRect(10, 100, 220, 80), "Quit", mRen));
	p13->Buttons()->back()->SetAdjacent(-1, 14, -1, 14);
	mScenes[GAME]->AddPanel(p13);
	Panel* p14 = new Panel(mRen, sf::FloatRect(sf::Vector2f(480, 320), sf::Vector2f(240, 190)));
	p14->SetCanFocus(true);
	Text* t2 = new Text("You Win", "detente.ttf");
	t2->setPosition(sf::Vector2f(10, 10));
	t2->setCharacterSize(20);
	p14->AddText(t2);
	p14->AddButton(new Button(sf::FloatRect(10, 10, 220, 80), "Next Level", mRen));
	p14->Buttons()->back()->SetAdjacent(-1, 17, -1, 17);
	p14->AddButton(new Button(sf::FloatRect(10, 100, 220, 80), "Quit", mRen));
	p14->Buttons()->back()->SetAdjacent(-1, 16, -1, 16);
	mScenes[GAME]->AddPanel(p14);
	mHudButtons = std::vector<Button*>();
	mHudButtons.insert(mHudButtons.end(), p->Buttons()->begin(), p->Buttons()->end());
	mHudButtons.insert(mHudButtons.end(), p2->Buttons()->begin(), p2->Buttons()->end());
	mHudButtons.insert(mHudButtons.end(), p3->Buttons()->begin(), p3->Buttons()->end());
	mHudButtons.insert(mHudButtons.end(), p4->Buttons()->begin(), p4->Buttons()->end());
	mHudButtons.insert(mHudButtons.end(), p13->Buttons()->begin(), p13->Buttons()->end());
	mHudButtons.insert(mHudButtons.end(), p14->Buttons()->begin(), p14->Buttons()->end());
	for (int i = 7; i < 12; i++)
		(*mScenes[2]->Panels())[i]->SetEnabled(false);
	(*mScenes[mScene]->Panels())[7 + mLevel->GetPlayer()->GetEquipped()]->SetEnabled(true);
	mCountdown = new Text("Press Enter To Begin", "detente.ttf");
	mCountdown->setPosition(sf::Vector2f(10, 10));
	mCountdown->setCharacterSize(20);
	mCountdown->setColor(sf::Color(210, 0, 0));
	mScenes[GAME]->AddText(mCountdown);
}

void Menu::UpdateTurretMenus()
{
	for (auto it = mTurretPanels.cbegin(); it != mTurretPanels.cend();)
	{
		if (std::find(mLevel->GetTurrets()->begin(), mLevel->GetTurrets()->end(), it->second) == mLevel->GetTurrets()->end())
		{
			auto i = std::find(mScenes[GAME]->Panels()->begin(), mScenes[GAME]->Panels()->end(), it->first);
			if (i != mScenes[GAME]->Panels()->end())
			{
				mScenes[GAME]->Panels()->erase(i);
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
			p->SetJoystickCanFocus(false);
			p->Buttons()->back()->SetFontSize(12);
			mScenes[GAME]->AddPanel(p);
		}
		p->SetPosition(sf::Vector2f((t->Location() - sf::Vector2f(18, 0) - mCam->Offset())*mCam->Scale()));
	}
}


bool Menu::ProcessInput(sf::Event e, int modifierKeys)
{
	mFocus = false;
	if (e.type == sf::Event::MouseMoved)
	{
		mPos = sf::Vector2i(e.mouseMove.x, e.mouseMove.y);
		mSelectedButton = -1;
	}
	if(e.type == sf::Event::MouseLeft)
	{
		mPos = sf::Vector2i(-1, -1);
	}
	else if (e.type == sf::Event::JoystickMoved)
	{
		if (mSelectedButton == -1)
			mSelectedButton = 0;
		if (mSelectedButton != -1)
		{
			int d = -1;
			bool xAxis = mXAxisReset, yAxis = mYAxisReset;
			if (e.joystickMove.axis == sf::Joystick::Axis::X)
			{
				xAxis = false;
				if (e.joystickMove.position > 25)
					d = 2;
				else if (e.joystickMove.position < -25)
					d = 0;
				else
					xAxis = true;
			}
			else if (e.joystickMove.axis == sf::Joystick::Axis::Y)
			{
				yAxis = false;
				if (e.joystickMove.position > 25)
					d = 3;
				else if (e.joystickMove.position < -25)
					d = 1;
				else
					yAxis = true;
			}
			if ((mXAxisReset && (d == 2 || d == 0)) || (mYAxisReset && (d == 1 || d == 3)))
				if ((*mSelectedButtons)[mSelectedButton]->GetAdjacent(d) != -1)
					if ((*mSelectedButtons)[(*mSelectedButtons)[mSelectedButton]->GetAdjacent(d)]->Visible())
						mSelectedButton = (*mSelectedButtons)[mSelectedButton]->GetAdjacent(d);
			mXAxisReset = xAxis;
			mYAxisReset = yAxis;
			mAxisReset = xAxis && yAxis;
		}
	}
	if (mScene == GAME)
	{
		if (e.type == sf::Event::JoystickButtonPressed)
		{
			if (e.joystickButton.button == Controller::X)
			{
				for (int i = 7; i < 12; i++)
				{
					if ((*mScenes[mScene]->Panels())[i]->Enabled())
					{
						(*mScenes[mScene]->Panels())[i]->SetEnabled(false);
						int i2 = (i + 1) > 11 ? 7 : i + 1;
						(*mScenes[mScene]->Panels())[i2]->SetEnabled(true);
						mHudRects[2]->first.setPosition(sf::Vector2f(477 + 50 * (i2 - 7), 852));
						break;
					}
				}
			}
		}
		else if (e.type == sf::Event::KeyPressed)
		{
			if (e.key.code == sf::Keyboard::Num1 || e.key.code == sf::Keyboard::Num2 || e.key.code == sf::Keyboard::Num3 || e.key.code == sf::Keyboard::Num4 || e.key.code == sf::Keyboard::Num5)
			{
				for (int i = 7; i < 12; i++)
				{
					(*mScenes[mScene]->Panels())[i]->SetEnabled(false);
				}
			}
			if (e.key.code == sf::Keyboard::Num1)
			{
				(*mScenes[mScene]->Panels())[7]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(477, 852));
			}
			else if (e.key.code == sf::Keyboard::Num2)
			{
				(*mScenes[mScene]->Panels())[8]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(527, 852));
			}
			else if (e.key.code == sf::Keyboard::Num3)
			{
				(*mScenes[mScene]->Panels())[9]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(577, 852));
			}
			else if (e.key.code == sf::Keyboard::Num4)
			{
				(*mScenes[mScene]->Panels())[10]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(627, 852));
			}
			else if (e.key.code == sf::Keyboard::Num5)
			{
				(*mScenes[mScene]->Panels())[11]->SetEnabled(true);
				mHudRects[2]->first.setPosition(sf::Vector2f(677, 852));
			}
		}
		else if (e.type == sf::Event::JoystickButtonReleased)
		{
			if (e.joystickButton.button == Controller::Y)
			{
				//toggles Upgrades panel
				bool upgradeVisible = (*mScenes[mScene]->Panels())[3]->GetVisible();
				(*mScenes[mScene]->Panels())[3]->SetVisible(!upgradeVisible);
				if (upgradeVisible)
				{
					(*mScenes[mScene]->Panels())[1]->SetVisible(false);
					(*mScenes[mScene]->Panels())[2]->SetVisible(false);
					mSelectedButton = 0;
				}
				else
				{
					(*mScenes[mScene]->Panels())[1]->SetVisible(true);
					mSelectedButton = 12;
				}
			}
			else if (e.joystickButton.button == Controller::START)
			{
				//toggles pause menu panel
				(*mScenes[mScene]->Panels())[0]->SetVisible(!(*mScenes[mScene]->Panels())[0]->GetVisible());
				mPauseMenu = (*mScenes[mScene]->Panels())[0]->GetVisible();
				mScenes[mScene]->SetCanFocus((*mScenes[mScene]->Panels())[0]->GetVisible());
			}
			else if (e.joystickButton.button == Controller::B)
			{
				//toggles Turret Upgrade Panels
				mTurretPanelsVis = !mTurretPanelsVis;
				for (auto it = mTurretPanels.cbegin(); it != mTurretPanels.cend(); it++)
					it->first->SetVisible(mTurretPanelsVis);
			}
			else if (e.joystickButton.button == Controller::A)
			{
				if (mSelectedTurret != 0){
					int level = mSelectedTurret->GetLevel();
					if (level < 4){
						int upgradeCost = mSelectedTurret->GetUpgradeCost(level);
						if (mLevel->GetPlayer()->GetCredits() >= upgradeCost){
							mLevel->GetPlayer()->AddCredits(-upgradeCost);
							mSelectedTurret->SetLevel(level + 1);
							upgradeCost = mSelectedTurret->GetUpgradeCost(level + 1);
							Button* b = 0;
							for (auto it = mTurretPanels.cbegin(); it != mTurretPanels.cend(); it++)
								if (it->second == mSelectedTurret)
								{
								b = (*it->first->Buttons())[0];
								break;
								}
							b->SetText("Lv. " + std::to_string(level + 2) + (level + 1 < 4 ? " (" + std::to_string(upgradeCost) + "C)" : " MAX"));
						}
					}
				}
			}
		}
		else if (e.type == sf::Event::KeyReleased)
		{
			if (e.key.code == sf::Keyboard::Escape)
			{
				//toggles pause menu panel
				(*mScenes[mScene]->Panels())[0]->SetVisible(!(*mScenes[mScene]->Panels())[0]->GetVisible());
				mPauseMenu = (*mScenes[mScene]->Panels())[0]->GetVisible();
				mScenes[mScene]->SetCanFocus((*mScenes[mScene]->Panels())[0]->GetVisible());
			}
			else if (e.key.code == sf::Keyboard::Tab && modifierKeys == ModifierKeys::None)
			{
				//toggles Upgrades panel
				bool upgradeVisible = (*mScenes[mScene]->Panels())[3]->GetVisible();
				(*mScenes[mScene]->Panels())[3]->SetVisible(!upgradeVisible);
				if (upgradeVisible)
				{
					(*mScenes[mScene]->Panels())[1]->SetVisible(false);
					(*mScenes[mScene]->Panels())[2]->SetVisible(false);
					mSelectedButton = 0;
				}
				else
				{
					(*mScenes[mScene]->Panels())[1]->SetVisible(true);
					mSelectedButton = 12;
				}
			}
			else if (e.key.code == sf::Keyboard::Q)
			{
				//toggles Turret Upgrade Panels
				mTurretPanelsVis = !mTurretPanelsVis;
				for (auto it = mTurretPanels.cbegin(); it != mTurretPanels.cend(); it++)
					it->first->SetVisible(mTurretPanelsVis);
			}
			else if (e.key.code == sf::Keyboard::F)
			{
				//Upgrades Selected Turret
				if (mSelectedTurret != 0){
					int level = mSelectedTurret->GetLevel();
					if (level < 4){
						int upgradeCost = mSelectedTurret->GetUpgradeCost(level);
						if (mLevel->GetPlayer()->GetCredits() >= upgradeCost){
							mLevel->GetPlayer()->AddCredits(-upgradeCost);
							mSelectedTurret->SetLevel(level + 1);
							upgradeCost = mSelectedTurret->GetUpgradeCost(level + 1);
							Button* b = 0;
							for (auto it = mTurretPanels.cbegin(); it != mTurretPanels.cend(); it++)
								if (it->second == mSelectedTurret)
								{
								b = (*it->first->Buttons())[0];
								break;
								}
							b->SetText("Lv. " + std::to_string(level + 2) + (level + 1 < 4 ? " (" + std::to_string(upgradeCost) + "C)" : " MAX"));
						}
					}
				}
			}
		}
	}
	mFocus = mScenes[mScene]->ProcessInput(e, modifierKeys);
	mWin->setMouseCursorVisible(mFocus);
	return mFocus;
}

void Menu::Update()
{
	mScenes[mScene]->Update(mPos, mSelectedButton);
	//checks if a scenes button has been pressed and
	//decides what to do using the buttons text
	Button* b;
	Slider* s;
	if ((b = mScenes[mScene]->SelectedButton()) != 0)
	{
		string text = b->GetText();
		if (mScene == GAME)
		{
			if (text.substr(0, 3) == "Lv.")
			{
				mSelectedTurret = mTurretPanels[b->GetPanel()];
			}
		}
	}
	if ((b = mScenes[mScene]->UnSelectedButton()) != 0)
	{
		string text = b->GetText();
		if (mScene == GAME)
		{
			if (text.substr(0, 3) == "Lv.")
			{
				mSelectedTurret = 0;
			}
		}
	}
	if ((b = mScenes[mScene]->DownButton()) != 0)
	{
		SoundManager::PlaySoundEffect("ButtonClick");
		string text = b->GetText();
		if (mScene == MAIN)
		{
			if (text == "Start")
			{
				SetScene(LEVELS);
			}
			else if (text == "Options")
			{
				SetScene(OPTIONS);
			}
			else if (text == "Quit")
			{
				mWin->close();
			}
		}
		else if (mScene == LEVELS)
		{
			if (text == "Back")
			{
				SetScene(MAIN);
			}
			else if (text.substr(0, 5) == "Level")
			{
				mLevel->LoadLevel(std::atoi(text.substr(6).c_str()));
				SetScene(GAME);
			}
			else if (text == "Join Game")
			{
				SetScene(SERVERS);
			}
		}
		else if (mScene == SERVERS)
		{
			if (text == "Back")
			{
				SetScene(LEVELS);
			}
			else if (text == "Refresh")
			{
				while ((*mScenes[SERVERS]->GetButtons()).size() > 2)
				{
					delete (mScenes[SERVERS]->GetButtons()->at(2));
					mScenes[SERVERS]->GetButtons()->erase(mScenes[SERVERS]->GetButtons()->begin() + 2);
				}

				string ss = Network::Get("servers/", std::vector<std::array<string, 2>>());
				if (ss != "")
				{
					ss.erase(std::remove(ss.begin(), ss.end(), '\"'), ss.end());
					ss.erase(std::remove(ss.begin(), ss.end(), ' '), ss.end());
					std::vector<string> servers = Network::Split(ss, "},");

					int i = 0;
					if (servers.size() > 0)
						mScenes[SERVERS]->GetButtons()->at(0)->SetAdjacent(1, 1, 2, 2);
					for (int i = 0; i < servers.size(); i++)
					{
						mScenes[SERVERS]->GetButtons()->at(1)->SetAdjacent(i+2, i+2, 0, 0);
						string s = servers[i];
						s.erase(std::remove(s.begin(), s.end(), '{'), s.end());
						s.erase(std::remove(s.begin(), s.end(), '}'), s.end());
						int start = s.find(':');
						string name = s.substr(0, start);
						s = s.erase(0, start + 1);
						std::vector<string> info = Network::Split(s, ",");
						std::map<string, string> inf;
						for (int i2 = 0; i2 < info.size(); i2++)
						{
							std::vector<string> detail = Network::Split(info[i2], ":");
							inf[detail[0]] = detail[1];
						}
						Button* b2 = new Button(sf::FloatRect(100, 100 + 42 * i, 500, 40), name + "    " + inf["count"] + "/2    " + inf["ip"], mRen);
						mScenes.back()->GetButtons()->back()->SetAdjacent(12, 7, 8, 0);
						//Button* b2 = new Button(sf::FloatRect(100, 100 + 42 * i, 500, 40), inf["ip"], mRen);
						b2->SetTag(inf["ip"]);
						b2->SetVisible(true);
						mScenes[SERVERS]->GetButtons()->back()->SetAdjacent(0, i, i + 2, 1);
						mScenes[SERVERS]->AddButton(b2);
						mScenes[SERVERS]->GetButtons()->back()->SetAdjacent(0, i + 1, 1, 1);
					}
				}
			}
			else
			{
				if (!Network::Connected())
				{
					Network::SendConnectMessage(b->Tag());
					sf::Packet packet = Network::UdpListen();
					while(packet.getDataSize() == 0)
						packet = Network::UdpListen();
					LoadMapPacket p = LoadMapPacket();
					byte type;
					packet >> type;
					if (type == LOADMAP){
						packet >> p;

						mLevel->LoadMultiplayerLevel(p.number);
						SetScene(GAME);
					}
				}
			}
		}
		else if (mScene == GAME)
		{
			if (text == "Resume")
			{
				(*mScenes[mScene]->Panels())[0]->SetVisible(!(*mScenes[mScene]->Panels())[0]->GetVisible());
				mPauseMenu = (*mScenes[mScene]->Panels())[0]->GetVisible();
				mScenes[mScene]->SetCanFocus((*mScenes[mScene]->Panels())[0]->GetVisible());
			}
			else if (text == "Options")
			{
				SetScene(OPTIONS);
			}
			else if (text == "Start Server")
			{
				if (b->IsToggleOn())
				{
					b->SetBackColor(sf::Color(40, 200, 90));
					std::vector<std::array<string, 2>> update;
					update.push_back({ { "type", "update" } });
					update.push_back({ { "name", (*mScenes[OPTIONS]->TextBoxes())[0]->GetValue() } });
					update.push_back({ { "count", std::to_string(mLevel->PlayerCount()) } });
					update.push_back({ { "ip", Network::GetLocalIP() } });
					cout << Network::Post("updateserver/", update) << endl;
					Network::StartTcpListen();
					LoadMapPacket p = LoadMapPacket();
					p.number = mLevel->CurrentLevel();
					sf::Packet packet = sf::Packet();
					packet << p;
					Network::SendUdp(packet);
					mLevel->AddPlayer2();
					update[2] = { { "count", std::to_string(mLevel->PlayerCount()) } };
					Network::Post("updateserver/", update);
				}
				else
				{
					b->SetBackColor(sf::Color(200, 100, 0));
					cout << Network::Post("updateserver/", { { { { "type", "delete" } }, { { "name", (*mScenes[OPTIONS]->TextBoxes())[0]->GetValue() } } } }) << endl;
				}

			}
			else if (text == "Retry")
			{
				mLevel->RestartCurrentWave();
			}
			else if (text == "Quit")
			{
				//mWin->close();
				mScenes[mScene]->SetCanFocus(false);
				SetScene(MAIN);
			}
			else if (text == "Next Level")
			{
				mLevel->LoadNextLevel();
			}
			else if (text == "Weapon")
			{
				(*mScenes[mScene]->Panels())[1]->SetVisible(true);
				(*mScenes[mScene]->Panels())[2]->SetVisible(false);
			}
			else if (text == "Suit")
			{
				(*mScenes[mScene]->Panels())[2]->SetVisible(true);
				(*mScenes[mScene]->Panels())[1]->SetVisible(false);
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
			else if (text.substr(0, 7) == "Grenade")
			{
				int level = mLevel->GetPlayer()->GetPouchLevel();
				if (level < 4){
					int upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level);
					if (mLevel->GetPlayer()->GetCredits() >= upgradeCost){
						mLevel->GetPlayer()->AddCredits(-upgradeCost);
						mLevel->GetPlayer()->SetPouchLevel(level + 1);
						upgradeCost = mLevel->GetPlayer()->GetUpgradeCost(level + 1);
						b->SetText("Grenade Lv. " + std::to_string(level + 2) + (level + 1 < 4 ? "\n(" + std::to_string(upgradeCost) + "C)" : "\nMAX"));
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
		else if (mScene == OPTIONS)
		{
			if (text == "Back")
			{
				SetScene(mPreviousScene);
			}
		}
		//window.setMouseCursorVisible(false);
	}
	else if ((s = mScenes[mScene]->ChangedSlider()) != 0)
	{
		string text = s->GetText();
		if (mScene == OPTIONS)
		{
			if (text == "Music Volume")
			{
				SoundManager::SetMusicVolume(s->GetValue());
			}
			else if (text == "Effect Volume")
			{
				SoundManager::SetEffectVolume(s->GetValue());
			}
		}
	}
	if (mScene == GAME)
	{
		//updates the HUD
		if (mTurretPanelsVis)
		{
			Turret* t = 0;
			float dist = 100000;
			for (auto it = mTurretPanels.begin(); it != mTurretPanels.end(); it++)
			{
				float tempDist = sqrt((it->second->Location().x - mLevel->GetPlayer()->Location().x)*(it->second->Location().x - mLevel->GetPlayer()->Location().x) + (it->second->Location().y - mLevel->GetPlayer()->Location().y)*(it->second->Location().y - mLevel->GetPlayer()->Location().y));
				if (tempDist < dist)
				{
					dist = tempDist;
					t = it->second;
				}
				if (it->first->SelectedButton() != 0)
				{
					t = 0;
					break;
				}
			}
			if (t != 0 || mTurretPanels.size() == 0)
				mSelectedTurret = t;
		}
		else
			mSelectedTurret = 0;
		std::pair<int, int> ammo = mLevel->GetPlayer()->GetGun()->GetAmmo();
		mHudTexts[0]->setString(std::to_string(ammo.first) + " / " + std::to_string(ammo.second));
		mHudTexts[1]->setString("Credits: " + std::to_string(mLevel->GetPlayer()->GetCredits()));
		mHudTexts[2]->setString(std::to_string(mLevel->GetWave()));
		mHudTexts[3]->setString(std::to_string(mLevel->GetPlayer()->GetGrenadeCount()));
		mHudRects[0]->first.setScale(mLevel->GetPlayer()->Health() / mLevel->GetPlayer()->GetMaxHealth(), 1);
		mHudRects[1]->first.setScale(mLevel->GetPlayer()->JetFuel() / mLevel->GetPlayer()->GetMaxFuel(), 1);
		UpdateTurretMenus();
		if (mLevel->GameOver())
			mSelectedButton = 14;
		else if (mLevel->Won())
			mSelectedButton = 16;
		(*mScenes[GAME]->Panels())[12]->SetVisible(mLevel->GameOver());
		(*mScenes[GAME]->Panels())[13]->SetVisible(mLevel->Won());
		SetPaused(mLevel->GameOver() || mLevel->Won() || mPauseMenu);
		for (auto it = mTurretPanels.begin(); it != mTurretPanels.end(); it++)
			it->second->Highlight(false);
		if(mSelectedTurret != 0)
		mSelectedTurret->Highlight(true);
	}

}

void Menu::SetScene(int i)
{
	mPreviousScene = mScene;
	mScene = i;
	if (mPreviousScene == GAME && mScene == MAIN)
	{
		SoundManager::PlayMusic("Menu");
		mLevel->ClearLevel();
	}
	mScenes[mPreviousScene]->SetVisible(false);
	if (mScene == MAIN || mScene == OPTIONS || mScene == LEVELS || mScene == SERVERS)
	{
		mSelectedButtons = mScenes[mScene]->GetButtons();
	}
	else if (mScene == GAME)
	{
		mSelectedButtons = &mHudButtons;
		if (mPreviousScene == LEVELS || mPreviousScene == SERVERS)
		{
			InitHud();
			mPauseMenu = false;
			mTurretPanelsVis = false;
		}
		else if (mPreviousScene == OPTIONS)
			(*mScenes[mScene]->Panels())[0]->SetVisible(true);
		for (int i = 4; i < (*mScenes[mScene]->Panels()).size(); i++)
		{
			(*mScenes[mScene]->Panels())[i]->SetVisible(true);
		}
	}
	mSelectedButton = -1;
	mScenes[mScene]->SetVisible(true);
	//mScenes[mScene]->SetVisible(mScene != GAME);
	SetPaused(mScene != GAME);
}

void Menu::SetPaused(bool b)
{
	if (mPaused != b){
		mPaused = b;
		mWin->setMouseCursorVisible(mPaused);
		mLevel->Pause(mPaused);
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