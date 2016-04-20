#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "Sprite.h"
#include "Renderer.h"
#include "Text.h"
#include <functional>

class TextBox
{
private:
	sf::FloatRect mRect;
	sf::FloatRect mTextRect;
	sf::FloatRect mCaretRect;
	std::pair<sf::RectangleShape, bool> mTextArea;
	std::pair<sf::RectangleShape, bool> mBackground;
	std::pair<sf::RectangleShape, bool> mCaret;
	std::pair<sf::RectangleShape, bool> mHighlight;
	std::vector<std::pair<sf::RectangleShape, bool >*> mBoundary;
	Text mText, mValueText;
	bool mVisible = false;
	bool mChanged = false;
	bool mDown = false, mSelected = false, mOut;
	Renderer* mRen;
	int mMaxChars, mIndex;
	string mString, mValue;
	sf::Clock mBlink;
	std::vector<int> mCharWidths;
	int mSelectLength;

public:
	TextBox();
	TextBox(sf::FloatRect, string, string, Renderer*, int);
	~TextBox();
	sf::FloatRect Rect();
	bool ValueChanged();
	void InitBoundary(Renderer*);
	void Offset(sf::Vector2f);
	void Update(sf::Vector2i, bool);
	void InputKey(sf::Keyboard::Key, int);
	void SetVisible(bool);
	void SetSelected(int, int);
	bool SetValue(string);
	void Offset(float, float);
	string GetText(), GetValue();
};


#endif