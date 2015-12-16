#ifndef TEXT_H
#define TEXT_H

class Text : public sf::Text
{
private:
	static std::map<std::string, std::pair<sf::Font, int>> mFonts;
	void RemoveFont(std::string);
	void AddFont(std::string);
	bool mVisible;
	std::string mPath = "";
public:
	Text(std::string, std::string);
	Text(const Text &obj);
	Text() : mVisible(true) {}
	bool Visible() const;
	void SetVisible(bool);
	~Text();
};

#endif