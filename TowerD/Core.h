#ifndef CORE_H
#define CORE_H
#include "SFML/Graphics.hpp" 
#include "Entity.h"	

class Core : public Entity
{
private:
public:
	Core(sf::Vector2f, int, Renderer*);
	 void Update(float);
	 void LoadAssets();
	//void Draw(sf::RenderWindow*, sf::Vector2f, float);
};


#endif