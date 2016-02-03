#include "stdafx.h"
#include "Sprite.h"

//wrapper class to simplify sprite/texture use
//also includes static map of textures to prevent
//the same file being loaded more than once
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

//removes texture from static map
void Sprite::RemoveTexture(std::string filepath)
{
	if (mTextures.count(filepath))
	{
		//also keeps track of how many sprites are using a texture
		//so that it can be unloaded when its no longer in use
		mTextures[mPath].second--;
		if (mTextures[mPath].second <= 0 && std::find(mKeep.begin(), mKeep.end(), filepath) == mKeep.end())
			mTextures.erase(mPath);
	}
}

//adds texture to static map and loads it if its not already there
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

//function for changing texture of a sprite
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

//allows sprites to be ignored by renderer
void Sprite::SetVisible(bool v)
{
	mVisible = v;
}

//allows you to prevent a texture from being unloaded even if unused
void Sprite::SetKeep(std::string filepath, bool k)
{
	if (std::find(mKeep.begin(), mKeep.end(), filepath) != mKeep.end() && !k)
	{
		if (!k)
			mKeep.erase(std::find(mKeep.begin(), mKeep.end(), filepath));
	}
	else
		if (k)
		{
			mKeep.push_back(filepath); 
			if (!mTextures.count(filepath))
			{
				sf::Texture t;
				t.loadFromFile(filepath);
				mTextures.insert(std::pair<std::string, std::pair<sf::Texture, int>>(filepath, std::pair<sf::Texture, int>(t, 0)));
			}
			mTextures[filepath].second++;
		}
}

Sprite::~Sprite()
{
	RemoveTexture(mPath);
}
std::map<std::string, std::pair<sf::Texture, int>> Sprite::mTextures;
std::vector<std::string> Sprite::mKeep;