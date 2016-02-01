#ifndef SPRITE_H
#define SPRITE_H
#include <map>

class Sprite : public sf::Sprite
{
private:
	static std::map<std::string, std::pair<sf::Texture, int>> mTextures;
	static std::vector<std::string> mKeep;
	std::string mPath = "";
	void RemoveTexture(std::string);
	void AddTexture(std::string);
	bool mVisible;
public:
	Sprite(std::string);
	Sprite(const Sprite &obj);
	Sprite() : mVisible(true) 
	{
	};
	void setTexture(std::string);
	bool Visible() const;
	void SetVisible(bool);
	static void SetKeep(std::string, bool);
	~Sprite();
};

#endif