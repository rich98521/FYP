#include "stdafx.h"
#include "Text.h"

//wrapper class to simplify text/font usage
Text::Text(std::string text, std::string font)
	:sf::Text(), mVisible(true), mPath(font)
{
	setString(text);
	AddFont(font);
	setCharacterSize(12);
}

void Text::RemoveFont(std::string filepath)
{
	if (mFonts.count(filepath))
	{
		mFonts[mPath].second--;
		mFonts.erase(mPath);
	}
}

void Text::AddFont(std::string filepath)
{
	if (!mFonts.count(filepath))
	{
		sf::Font f;
		f.loadFromFile("D:\\Projects\\TowerD\\Fonts\\" + filepath);
		mFonts.insert(std::pair<std::string, std::pair<sf::Font, int>>(filepath, std::pair<sf::Font, int>(f, 0)));
	}
	mFonts[filepath].second++;
	sf::Text::setFont(mFonts[filepath].first);
}

bool Text::Visible() const
{
	return mVisible;
}

void Text::SetVisible(bool v)
{
	mVisible = v;
}

Text::~Text()
{
	RemoveFont(mPath);
}

std::map<std::string, std::pair<sf::Font, int>> Text::mFonts;