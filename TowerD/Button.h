#ifndef BUTTON_H
#define BUTTON_H
#include "Sprite.h"
#include "Renderer.h"
#include "Text.h"
#include <functional>


class Button
{
private:
	sf::IntRect mRect;
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


public:
	Button();
	Button(sf::IntRect, string, Renderer*);
	Button(sf::IntRect, string, float, Renderer*);
	Button(sf::IntRect, string, string, Renderer*);
	sf::IntRect Rect();
	bool IsClicked();
	void InitBoundary(Renderer*);
	void Offset(sf::Vector2f);
	void Update(sf::Vector2i, bool);
	void SetVisible(bool);
	string GetText();
};


#endif