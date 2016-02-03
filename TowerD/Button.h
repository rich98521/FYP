#ifndef BUTTON_H
#define BUTTON_H
#include "Sprite.h"
#include "Renderer.h"
#include "Text.h"
#include <functional>

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
	void Update(sf::Vector2i, bool);
	void SetVisible(bool);
	string GetText();
	void Offset(float, float);
	void SetText(string);
	void SetFontSize(float);
	void SetPanel(Panel* p){ mPanel = p; }
	Panel* GetPanel(){ return mPanel; }
};


#endif