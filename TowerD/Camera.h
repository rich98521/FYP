#ifndef CAMERA_H
#define CAMERA_H
#include "SFML/Graphics.hpp" 


class Camera
{
private:
	sf::Vector2f mOffset;
	sf::Vector2i* mAimPos;
	sf::Vector2f* mPlayerPos;
	float mScale = 0;
public:
	Camera(sf::Vector2i*, sf::Vector2f*);
	Camera(){};
	sf::Vector2f Offset() const;
	void Update(sf::Vector2i);
	float Scale() const;
	void SetScale(float);
};


#endif