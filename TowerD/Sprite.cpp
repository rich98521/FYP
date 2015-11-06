#include "stdafx.h"
#include "Sprite.h"

Sprite::Sprite(std::string filepath)
	:sf::Sprite(), mPath(filepath), mVisible(true)
{
	AddTexture(filepath);
}

Sprite::Sprite(const Sprite &obj)
	: sf::Sprite(obj), mVisible(true)
{
	mPath = obj.mPath;
	if (mPath != "")
		AddTexture(mPath);
}

void Sprite::RemoveTexture(std::string filepath)
{
	if (mTextures.count(filepath))
	{
		mTextures[mPath].second--;
		if (mTextures[mPath].second <= 0)
			mTextures.erase(mPath);
	}
}

void Sprite::AddTexture(std::string filepath)
{
	if (!mTextures.count(filepath))
	{
		sf::Texture t;
		t.loadFromFile(filepath);
		mTextures.insert(std::pair<std::string, std::pair<sf::Texture, int>>(filepath, std::pair<sf::Texture, int>(t, 0)));
	}
	mTextures[filepath].second++;
	sf::Sprite::setTexture(mTextures[filepath].first);
}

void Sprite::setTexture(std::string filepath)
{
	RemoveTexture(mPath);
	mPath = filepath;
	AddTexture(mPath);
}

bool Sprite::Visible() const
{
	return mVisible;
}

void Sprite::SetVisible(bool v)
{
	mVisible = v;
}

Sprite::~Sprite()
{
	RemoveTexture(mPath);
}
std::map<std::string, std::pair<sf::Texture, int>> Sprite::mTextures;