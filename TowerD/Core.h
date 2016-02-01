#ifndef CORE_H
#define CORE_H
#include "Entity.h"	

class Core : public Entity
{
private:
public:
	Core(sf::Vector2f, int, Renderer*);
	void Update(float, sf::Vector2f, float);
	 void LoadAssets();
	//void Draw(sf::RenderWindow*, sf::Vector2f, float);
};


#endif