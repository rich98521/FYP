#ifndef SLIDER_H
#define SLIDER_H
#include "Sprite.h"
#include "Renderer.h"
#include "Text.h"
#include <functional>

class Slider
{
private:
	sf::FloatRect mRect;
	sf::FloatRect mSlideRect;
	sf::FloatRect mBarRect;
	std::pair<sf::RectangleShape, bool> mBar;
	std::pair<sf::RectangleShape, bool> mSlider;
	std::pair<sf::RectangleShape, bool> mBackground;
	std::vector<std::pair<sf::RectangleShape, bool >*> mBoundary;
	Text mValueText, mText;
	bool mVisible = false;
	bool mChanged = false;
	bool mDown = false, mDrag;
	Renderer* mRen;
	int mValue, mMaxValue;
	int mStartPos, mOffset;
	string mString;

public:
	Slider();
	Slider(sf::FloatRect, string, Renderer*, int);
	~Slider();
	sf::FloatRect Rect();
	bool ValueChanged();
	void InitBoundary(Renderer*);
	void Offset(sf::Vector2f);
	void Update(sf::Vector2i, bool);
	void SetVisible(bool);
	int GetValue();
	void SetValue(int);
	void SetMaxValue(int);
	void Offset(float, float);
	string GetText();
};


#endif