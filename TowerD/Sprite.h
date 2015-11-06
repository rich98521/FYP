#ifndef SPRITE_H
#define SPRITE_H
#include "SFML/Graphics.hpp" 
#include <map>

class Sprite : public sf::Sprite
{
private:
	static std::map<std::string, std::pair<sf::Texture, int>> mTextures;
	std::string mPath = "";
	void RemoveTexture(std::string);
	void AddTexture(std::string);
	bool mVisible;
public:
	Sprite(std::string);
	Sprite(const Sprite &obj);
	Sprite() : mVisible(true) {};
	void setTexture(std::string);
	bool Visible() const;
	void SetVisible(bool);
	~Sprite();
};

#endif