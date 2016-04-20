#include "stdafx.h"
#include "Slider.h"

//button just containing text
Slider::Slider(sf::FloatRect r, string text, Renderer* ren, int maxVal) : mRect(r), mValueText(std::to_string(mValue), "detente.ttf"), mText(text, "detente.ttf"), mRen(ren)
{
	mValueText.setColor(sf::Color(255, 255, 255, 255));
	mValueText.setCharacterSize(r.height / 2.f);
	mMaxValue = maxVal;
	SetValue(maxVal);
	mValueText.setPosition(mRect.left + mRect.width - mValueText.getLocalBounds().width - 3, r.top + (r.height - mValueText.getLocalBounds().height) / 2.f - mValueText.getLocalBounds().top);
	mText.setColor(sf::Color(255, 255, 255, 255));
	mText.setCharacterSize(r.height / 2.f);
	mText.setPosition(r.left - mText.getLocalBounds().width - 5, r.top + (r.height - mText.getLocalBounds().height) / 2.f - mText.getLocalBounds().top);
	mBackground.first.setFillColor(sf::Color(20, 20, 20, 255));
	mBackground.first.setPosition(mRect.left, mRect.top);
	mBackground.first.setSize(sf::Vector2f(mRect.width, mRect.height));
	mBarRect = sf::FloatRect(mRect.left + 10, mRect.top + (mRect.height - 8) / 2, mRect.width - mValueText.getLocalBounds().width - 15, 8);
	mBar.first.setFillColor(sf::Color(255, 255, 255, 255));
	mBar.first.setSize(sf::Vector2f(mBarRect.width, mBarRect.height));
	mBar.first.setPosition(mBarRect.left, mBarRect.top);
	mSlideRect = sf::FloatRect(0, mRect.top + (mRect.height - (mRect.height - 8)) / 2, 8, mRect.height - 8);
	mSlideRect.left = mBarRect.left + mBarRect.width - mSlideRect.width;
	mSlider.first.setFillColor(sf::Color(210, 100, 0, 255));
	mSlider.first.setPosition(mSlideRect.left, mSlideRect.top);
	mSlider.first.setSize(sf::Vector2f(mSlideRect.width, mSlideRect.height));
	ren->Add(&mBackground);
	ren->Add(&mBar);
	ren->Add(&mSlider);
	ren->Add(&mValueText);
	ren->Add(&mText);
	InitBoundary(ren);
	SetVisible(false);
	mChanged = false;
	mString = text;
}

void Slider::Offset(float x, float y)
{
	mRect.left += x;
	mRect.top += y;
	mValueText.setPosition(mValueText.getPosition() + sf::Vector2f(x, y));
	mText.setPosition(mText.getPosition() + sf::Vector2f(x, y));
	mBackground.first.setPosition(mBackground.first.getPosition() + sf::Vector2f(x, y));
	mSlider.first.setPosition(mSlider.first.getPosition() + sf::Vector2f(x, y));
	mBar.first.setPosition(mBar.first.getPosition() + sf::Vector2f(x, y));
	for (int i = 0; i < 4; i++)
	{
		(*mBoundary[i]).first.setPosition((*mBoundary[i]).first.getPosition() + sf::Vector2f(x, y));
	}
}

int Slider::GetValue()
{
	return mValue;
}

void Slider::SetValue(int v)
{
	mValue = fmin(v, mMaxValue);
	mValueText.setString(std::to_string(v));
	mValueText.setPosition(mRect.left + mRect.width - mValueText.getLocalBounds().width - 3, mRect.top + (mRect.height - mValueText.getLocalBounds().height) / 2.f - mValueText.getLocalBounds().top);
	mChanged = true;
}

void Slider::SetMaxValue(int v)
{
	mMaxValue = v;
}

sf::FloatRect Slider::Rect()
{
	return mRect;
}

bool Slider::ValueChanged()
{
	bool ans = mChanged;
	mChanged = false;
	return ans;
}

void Slider::InitBoundary(Renderer* ren)
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

void Slider::Offset(sf::Vector2f o)
{
	mRect.left += o.x;
	mRect.top += o.y;
	for (int i = 0; i < 4; i++)
		mBoundary[i]->first.setPosition(mBoundary[i]->first.getPosition() + o);
	mBackground.first.setPosition(mBackground.first.getPosition() + o);
	mValueText.setPosition(mValueText.getPosition() + o);
	mText.setPosition(mText.getPosition() + o);
	mSlider.first.setPosition(mSlider.first.getPosition() + o);
	mBar.first.setPosition(mBar.first.getPosition() + o);
}

//checks if mouse was first down and then up on the button 
//for button to be clicked
void Slider::Update(sf::Vector2i m, bool down)
{
	if (mVisible)
	{
		if (mDrag)
		{
			int start = mSlideRect.left;
			mSlideRect.left = m.x - mOffset;
			mSlideRect.left = fmin(fmax(mBarRect.left, mSlideRect.left), mBarRect.left + mBarRect.width - mSlideRect.width);
			if (mSlideRect.left != start)
			{
				mSlider.first.setPosition(mSlideRect.left, mSlideRect.top);
				SetValue(((mSlideRect.left - mBarRect.left) / (mBarRect.width - mSlideRect.width))*mMaxValue);
			}
		}
		bool contained = mSlideRect.contains(sf::Vector2f(m));
		if (down && contained && !mDown)
		{
			mOffset = m.x - mSlideRect.left;
			mDrag = true;
		}
		if (!down)
			mDrag = false;
		mDown = down;
	}
}

void Slider::SetVisible(bool v)
{
	mVisible = v;
	for each (std::pair<sf::RectangleShape, bool >* s in mBoundary)
	{
		s->second = v;
	}
	mValueText.SetVisible(v);
	mText.SetVisible(v);
	mBackground.second = v;
	mSlider.second = v;
	mBar.second = v;
}

string Slider::GetText()
{
	return mString;
}


Slider::~Slider()
{
	for each(std::pair<sf::RectangleShape, bool >* b in mBoundary)
	{
		mRen->Remove(b);
		delete(b);
	}
	mRen->Remove(&mBackground);
	mRen->Remove(&mSlider);
	mRen->Remove(&mBar);
	mRen->Remove(&mValueText);
	mRen->Remove(&mText);
}