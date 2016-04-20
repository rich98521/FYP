#include "stdafx.h"
#include "TextBox.h"

//button just containing text
TextBox::TextBox(sf::FloatRect r, string text, string value, Renderer* ren, int maxChars) : mRect(r), mValueText(value, "detente.ttf"), mText(text, "detente.ttf"), mRen(ren)
{
	mValueText.setColor(sf::Color(20, 20, 20, 255));
	mValueText.setCharacterSize(r.height / 2.f);
	mMaxChars = maxChars;
	SetValue(value);
	mText.setColor(sf::Color(255, 255, 255, 255));
	mText.setCharacterSize(r.height / 2.f);
	mText.setPosition(r.left - mText.getLocalBounds().width - 5, r.top + (r.height - mText.getLocalBounds().height) / 2.f - mText.getLocalBounds().top);
	mBackground.first.setFillColor(sf::Color(20, 20, 20, 255));
	mBackground.first.setPosition(mRect.left, mRect.top);
	mBackground.first.setSize(sf::Vector2f(mRect.width, mRect.height));
	mTextRect = sf::FloatRect(mRect.left + 3, mRect.top + 3, mRect.width - 6, mRect.height - 6);
	mTextArea.first.setFillColor(sf::Color(255, 255, 255, 255));
	mTextArea.first.setSize(sf::Vector2f(mTextRect.width, mTextRect.height));
	mTextArea.first.setPosition(mTextRect.left, mTextRect.top);
	mValueText.setPosition(mTextRect.left + 2, mTextRect.top + (mTextRect.height - mValueText.getLocalBounds().height) / 2.f - mValueText.getLocalBounds().top);
	mCaretRect = sf::FloatRect(0, mTextRect.top + 2, 2, mTextRect.height - 4);
	mCaret.first.setFillColor(sf::Color(210, 100, 0, 255));
	mCaret.first.setPosition(mCaretRect.left, mCaretRect.top);
	mCaret.first.setSize(sf::Vector2f(mCaretRect.width, mCaretRect.height));
	mHighlight.first.setFillColor(sf::Color(0, 100, 250, 255));
	mHighlight.first.setSize(sf::Vector2f(0, mCaretRect.height));
	ren->Add(&mBackground);
	ren->Add(&mTextArea);
	ren->Add(&mValueText);
	ren->Add(&mHighlight);
	ren->Add(&mCaret);
	ren->Add(&mText);
	InitBoundary(ren);
	SetVisible(false);
	mChanged = false;
	mString = text;
}

void TextBox::Offset(float x, float y)
{
	mRect.left += x;
	mRect.top += y;
	mValueText.setPosition(mValueText.getPosition() + sf::Vector2f(x, y));
	mText.setPosition(mText.getPosition() + sf::Vector2f(x, y));
	mBackground.first.setPosition(mBackground.first.getPosition() + sf::Vector2f(x, y));
	mTextArea.first.setPosition(mTextArea.first.getPosition() + sf::Vector2f(x, y));
	mCaret.first.setPosition(mCaret.first.getPosition() + sf::Vector2f(x, y));
	mHighlight.first.setPosition(mHighlight.first.getPosition() + sf::Vector2f(x, y));
	for (int i = 0; i < 4; i++)
	{
		(*mBoundary[i]).first.setPosition((*mBoundary[i]).first.getPosition() + sf::Vector2f(x, y));
	}
}

string TextBox::GetValue()
{
	return mValue;
}

bool TextBox::SetValue(string v)
{
	if (v != mValue && v.size() <= mMaxChars)
	{
		mCharWidths.clear();
		mCharWidths.push_back(0);
		sf::Text t(mValueText);
		t.setString(v[0]);
		for (int i = 0; i < v.size(); i++, t.setString(v.substr(0, i + 1)))
			mCharWidths.push_back(t.getLocalBounds().width);
		mValue = v;
		mValueText.setString(mValue);
		mChanged = true;
		return true;
	}
	else
		return false;
}

sf::FloatRect TextBox::Rect()
{
	return mRect;
}

bool TextBox::ValueChanged()
{
	bool ans = mChanged;
	mChanged = false;
	return ans;
}

void TextBox::InitBoundary(Renderer* ren)
{
	for (int i = 0; i < 4; i++)
	{
		mBoundary.push_back(new std::pair<sf::RectangleShape, bool >);
		mBoundary[i]->first.setFillColor(sf::Color(120, 120, 120, 255));
		ren->Add(mBoundary[i]);
	}
	mBoundary[0]->first.setPosition(mRect.left, mRect.top);
	mBoundary[0]->first.setSize(sf::Vector2f(mRect.width, 1));
	mBoundary[1]->first.setPosition(mRect.left, mRect.top + mRect.height - 1);
	mBoundary[1]->first.setSize(sf::Vector2f(mRect.width, 1));
	mBoundary[2]->first.setPosition(mRect.left, mRect.top);
	mBoundary[2]->first.setSize(sf::Vector2f(1, mRect.height));
	mBoundary[3]->first.setPosition(mRect.left + mRect.width - 1, mRect.top);
	mBoundary[3]->first.setSize(sf::Vector2f(1, mRect.height));
}

void TextBox::Offset(sf::Vector2f o)
{
	mRect.left += o.x;
	mRect.top += o.y;
	for (int i = 0; i < 4; i++)
		mBoundary[i]->first.setPosition(mBoundary[i]->first.getPosition() + o);
	mBackground.first.setPosition(mBackground.first.getPosition() + o);
	mValueText.setPosition(mValueText.getPosition() + o);
	mText.setPosition(mText.getPosition() + o);
	mTextArea.first.setPosition(mTextArea.first.getPosition() + o);
	mCaret.first.setPosition(mCaret.first.getPosition() + o);
	mHighlight.first.setPosition(mHighlight.first.getPosition() + o);
}

//checks if mouse was first down and then up on the button 
//for button to be clicked
void TextBox::Update(sf::Vector2i m, bool down)
{
	if (mVisible)
	{
		bool contained = mTextRect.contains(sf::Vector2f(m));
		if (mSelected)
		{
			if (mBlink.getElapsedTime().asSeconds() > .5f){
				mCaret.second = !mCaret.second;
				mBlink.restart();
			}
			if (down && mDown)
			{
				unsigned int i = 0;
				for (; i < mCharWidths.size(); i++)
				{
					int width = mCharWidths[i] + (mCharWidths[min(i + 1, mCharWidths.size() - 1)] - mCharWidths[i]) / 2;
					if (mValueText.getGlobalBounds().left + width > m.x)
						break;
				}
				i = min(i, mCharWidths.size() - 1);
				int index = mIndex, length = mSelectLength;
				if (length < -500)
					return;
				length += mIndex - i;
				index = i;
				SetSelected(index, length);
			}
		}
		if (down)
		{
			if (!contained)
			{
				mOut = true;
				if (!mDown){
					mSelected = false;
					mCaret.second = false;
					mHighlight.second = false;
				}
			}
		}
		else 
			mOut = false;
		if (!mOut)
		{
			if (mDown && !down)
			{
				if(contained)
					mSelected = true;
				else
					mCaret.second = false;
			}
			else if (down && !mDown && contained)
			{
				unsigned int i = 0;
				for (; i < mCharWidths.size(); i++)
				{
					int width = mCharWidths[i] + (mCharWidths[min(i + 1, mCharWidths.size() - 1)] - mCharWidths[i]) / 2;
					if (mValueText.getGlobalBounds().left + width > m.x)
						break;
				}
				int length = 0;
				SetSelected(i, length);
			}
			mDown = down;
		}
	}
}

void toClipboard(const std::string &s){
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
	if (!hg){
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

std::string fromClipboard()
{
	if (!OpenClipboard(nullptr))
		return "";
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
		return "";
	char * pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
		return "";
	std::string text(pszText);
	GlobalUnlock(hData);
	CloseClipboard();
	return text;
}

void TextBox::InputKey(sf::Keyboard::Key keyCode, int modifierKeys)
{
	
	if (mSelected)
	{
		int index = mIndex, length = mSelectLength;
		string value = mValue;
		string c = "";
		int start = mIndex, len = mSelectLength;
		if (len < 0)
		{
			start += len;
			len *= -1;
		}
		if ((mSelectLength != 0 && keyCode < 36 || keyCode == sf::Keyboard::BackSpace || keyCode == sf::Keyboard::Delete) && !(modifierKeys == ModifierKeys::Control && (keyCode == sf::Keyboard::C || keyCode == sf::Keyboard::A)))
		{
			value.erase(start, len);
			length = 0;
			index = start;
		}
		if (keyCode >= 0 && keyCode < 36)
		{
			if (modifierKeys == 0){
				if (keyCode < 26)
					c += (char)(65 + keyCode);
				else
					c = std::to_string(keyCode - 26);
				if (value.size() + c.size() <= mMaxChars)
					index++;
				else
					c = "";
			}
			else
			{
				if (modifierKeys == ModifierKeys::Control)
				{
					if (keyCode == sf::Keyboard::C)
					{
						toClipboard(value.substr(start, len) + " ");
					}
					else if (keyCode == sf::Keyboard::V)
					{
						c = fromClipboard();
						if (value.size() + c.size() <= mMaxChars)
							index += c.size();
						else
							c = "";
					}
					else if (keyCode == sf::Keyboard::A)
					{
						index = value.size();
						length = value.size();
						length *= -1;
					}
				}
			}
		}
		else if (keyCode == sf::Keyboard::Space)
		{
			c = " ";
			if (value.size() + c.size() <= mMaxChars)
				index++;
			else
				c = "";
		}
		else if (keyCode == sf::Keyboard::Left)
		{
			if (index - 1 >= 0)
			{
				index = mIndex - 1;
				if (modifierKeys == ModifierKeys::Shift)
					length++;
			}
		}
		else if (keyCode == sf::Keyboard::Right)
		{
			if (index + 1 <= mValue.size())
			{
				index = mIndex + 1;
				if (modifierKeys == ModifierKeys::Shift)
					length--;
			}
		}
		else if (keyCode == sf::Keyboard::BackSpace)
		{
			if (mIndex > 0 && mSelectLength == 0)
			{
				value.erase(mIndex - 1, 1);
				index--;
			}
		}
		else if (keyCode == sf::Keyboard::Delete && mSelectLength == 0)
			value.erase(mIndex, 1);
		if (value.size() <= mMaxChars && c != "")
			value.insert(index - c.size(), c);
		SetValue(value);
		SetSelected(index, length);
	}
}

void TextBox::SetSelected(int i, int l)
{
	int index = mIndex, length = mSelectLength;
	mIndex = max(min(i, (int)mValue.size()), 0);
	mSelectLength = l;
	if (mIndex != index || mSelectLength != length)
	{
		mCaretRect.left = max(mValueText.getGlobalBounds().left + mCharWidths[mIndex] - (mCaretRect.width / 2), mValueText.getGlobalBounds().left);
		mCaret.first.setPosition(sf::Vector2f(mCaretRect.left, mCaretRect.top));
		mCaret.second = true;
		mHighlight.second = true;
		mHighlight.first.setPosition(sf::Vector2f(mValueText.getGlobalBounds().left + mCharWidths[mIndex], mCaretRect.top));
		int size = 0;
		if (mSelectLength != 0)
		{
			int dir = (mSelectLength / abs(mSelectLength));
			for (int i2 = mIndex; i2 != mIndex + mSelectLength; i2 += dir)
				size += (mCharWidths[min(i2 - ((-dir + 1) / 2) + 1, (int)mCharWidths.size() - 1)] - mCharWidths[i2 - ((-dir + 1) / 2)]) * dir;
		}
		mHighlight.first.setSize(sf::Vector2f(size, mCaretRect.height));
		mBlink.restart();
	}
}

void TextBox::SetVisible(bool v)
{
	mVisible = v;
	for each (std::pair<sf::RectangleShape, bool >* s in mBoundary)
	{
		s->second = v;
	}
	mValueText.SetVisible(v);
	mText.SetVisible(v);
	mBackground.second = v;
	mTextArea.second = v;
	mCaret.second = v;
}

string TextBox::GetText()
{
	return mString;
}


TextBox::~TextBox()
{
	for each(std::pair<sf::RectangleShape, bool >* b in mBoundary)
	{
		mRen->Remove(b);
		delete(b);
	}
	mRen->Remove(&mBackground);
	mRen->Remove(&mTextArea);
	mRen->Remove(&mCaret);
	mRen->Remove(&mValueText);
	mRen->Remove(&mText);
}