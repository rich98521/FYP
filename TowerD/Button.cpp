#include "stdafx.h"
#include "Button.h"

//button just containing text
Button::Button(sf::IntRect r, string text, Renderer* ren) : mRect(r), mText(text, "detente.ttf")
{
	mString = text;
	mText.setColor(sf::Color(0, 0, 0, 255));
	mText.setCharacterSize(r.height / 2.f);
	mText.setPosition(r.left + (r.width - mText.getLocalBounds().width) / 2.f, r.top + (r.height - mText.getLocalBounds().height) / 2.f - mText.getLocalBounds().top);
	mOverlay.first.setPosition(r.left, r.top);
	mOverlay.first.setSize(sf::Vector2f(r.width, r.height));
	mBackground.first.setFillColor(sf::Color(192, 192, 192, 255));
	mBackground.first.setPosition(mRect.left, mRect.top);
	mBackground.first.setSize(sf::Vector2f(mRect.width, mRect.height));
	mOverlay.first.setFillColor(sf::Color(160, 160, 160, 128));
	ren->Add(&mBackground);
	ren->Add(&mOverlay);
	ren->Add(&mText);
	InitBoundary(ren);
	SetVisible(false);
}

//button with an image
Button::Button(sf::IntRect r, string text, string imPath, Renderer* ren) : mRect(r), mText(text, "detente.ttf")
{
	mString = text;
	mPictureText.loadFromFile(imPath);
	mPicture.first.setSize(sf::Vector2f(mPictureText.getSize()));
	mText.setColor(sf::Color(0, 0, 0, 255));
	mText.setCharacterSize((r.height - mPicture.first.getLocalBounds().height) / 4.f);
	mText.setPosition(r.left + (r.width - mText.getLocalBounds().width) / 2.f, r.top + r.height - (mText.getLocalBounds().height * 1.4));
	mOverlay.first.setPosition(r.left, r.top);
	mOverlay.first.setSize(sf::Vector2f(r.width, r.height));
	mPicture.first.setTexture(&mPictureText);
	mPicture.first.setPosition(r.left + (r.width - mPicture.first.getLocalBounds().width) / 2.f, r.top + (r.height - mText.getLocalBounds().height - mPicture.first.getLocalBounds().height) / 2.f);
	mBackground.first.setFillColor(sf::Color(192, 192, 192, 255));
	mBackground.first.setPosition(mRect.left, mRect.top);
	mBackground.first.setSize(sf::Vector2f(mRect.width, mRect.height));
	mOverlay.first.setFillColor(sf::Color(160, 160, 160, 64));
	ren->Add(&mBackground);
	ren->Add(&mPicture);
	ren->Add(&mOverlay);
	ren->Add(&mText);
	InitBoundary(ren);
	SetVisible(false);
}

string Button::GetText()
{
	return mString;
}

sf::IntRect Button::Rect()
{
	return mRect;
}

bool Button::IsClicked()
{
	bool ans = mClicked;
	mClicked = false;
	return ans;
}

void Button::InitBoundary(Renderer* ren)
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

void Button::Offset(sf::Vector2f o)
{
	mRect.left += o.x;
	mRect.top += o.y;
	for (int i = 0; i < 4; i++)
		mBoundary[i]->first.setPosition(mBoundary[i]->first.getPosition() + o);
	mPicture.first.setPosition(mPicture.first.getPosition() + o);
	mOverlay.first.setPosition(mOverlay.first.getPosition() + o);
	mBackground.first.setPosition(mBackground.first.getPosition() + o);
	mText.setPosition(mText.getPosition() + o);
}

//checks if mouse was first down and then up on the button 
//for button to be clicked
void Button::Update(sf::Vector2i m, bool down)
{
	if (mVisible)
	{
		bool contained = mRect.contains(m);
		if (!down)
			mOut = false;
		if (down && !contained)
			mOut = true;
		mOverlay.second = !contained;
		if (!mOut){
			if (mDown && !down && contained)
				mClicked = true;
			mDown = down;
		}
	}
}

void Button::SetVisible(bool v)
{
	mVisible = v;
	mPicture.second = v;
	for each (std::pair<sf::RectangleShape, bool >* s in mBoundary)
	{
		s->second = v;
	}
	mOverlay.second = v;
	mText.SetVisible(v);
	mBackground.second = v;
}




