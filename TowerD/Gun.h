#ifndef GUN_H
#define GUN_H
#include "SFML/Graphics.hpp" 
#include "Sprite.h" 
#include "Renderer.h" 


class Gun
{
private:
	float mAccuracy = 0.98f;
	float mDamage = 1;
	float mRate = 0.15f;
	int mMag = 20;
	int mMagTotal = 20;
	float mRange = 480;
	int mBulletCount = 1;
	sf::Clock mClock;
	Renderer* mRen;
	bool mShot;
	std::vector<Sprite*> mBulletSprites;
	std::vector<sf::Vector2f> mBulletHitPos;
public:
	Gun(Renderer*);
	std::vector<sf::Vector2f> Shoot(sf::Vector2f, float, sf::Vector2f, float);
	std::vector<sf::Vector2f> GetBullets() const;
	void Collision(int, float);
	void Draw();
	void Reload();
};


#endif