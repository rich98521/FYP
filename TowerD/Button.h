#ifndef BUTTON_H
#define BUTTON_H
#include "Sprite.h"
#include "Renderer.h"
#include "Text.h"
#include "SoundManager.h"
#include <functional>
#include <array>

class Panel;

class Button
{
private:
	sf::FloatRect mRect;
	sf::Texture mPictureText;
	std::pair<sf::RectangleShape, bool> mPicture;
	std::pair<sf::RectangleShape, bool> mOverlay;
	std::pair<sf::RectangleShape, bool> mBackground;
	std::vector<std::pair<sf::RectangleShape, bool >*> mBoundary;
	Text mText;
	bool mVisible = false;
	bool mClicked = false;
	bool mDown = false, mOut = false;
	string mString;
	Renderer* mRen;
	int mType = 0;
	Panel* mPanel = 0;
	std::array<int, 4> mAdjacentButtons{ { -1, -1, -1, -1 } };
	bool mSelected, mUnSelected, mToggle;
	string mTag;

public:
	Button();
	Button(sf::FloatRect, string, Renderer*);
	Button(sf::FloatRect, string, float, Renderer*);
	Button(sf::FloatRect, string, string, Renderer*);
	~Button();
	sf::FloatRect Rect();
	bool IsClicked();
	void InitBoundary(Renderer*);
	void Offset(sf::Vector2f);
	void Update(sf::Vector2i, bool, bool);
	void SetVisible(bool);
	bool Visible(){ return mVisible; };
	string GetText();
	void Offset(float, float);
	void SetText(string);
	void SetFontSize(float);
	void SetPanel(Panel* p){ mPanel = p; }
	void SetAdjacent(int l, int u, int r, int d){ mAdjacentButtons = { { l, u, r, d } }; };
	int GetAdjacent(int i){ return mAdjacentButtons[i]; };
	Panel* GetPanel(){ return mPanel; }
	bool IsSelected(){ return mSelected; };
	bool UnSelected(){ return mUnSelected; };
	bool IsToggleOn(){ return mToggle; };
	void SetBackColor(sf::Color c){ mBackground.first.setFillColor(c); };
	void SetTag(string s){ mTag = s; }
	string Tag(){ return mTag; }
};


#endif