#ifndef GUN_H
#define GUN_H
#include "Sprite.h" 
#include "Renderer.h" 


class Gun
{
private:
	float mAccuracy = 0.98f;
	//float mAccuracy = 1;
	float mDamage = 1;
	float mRate = 0.15f;
	int mMag;
	int mMagTotal = 20;
	float mRange = 480, mDrawRange = 480;
	int mBulletCount = 1;
	sf::Clock mClock;
	Renderer* mRen;
	bool mShot;
	std::vector<Sprite*> mBulletSprites;
	std::vector<sf::Vector2f*> mBulletHitPos;
public:
	Gun(Renderer*, int);
	std::vector<sf::Vector2f*> Shoot(sf::Vector2f, float);
	std::vector<sf::Vector2f*> GetBullets();
	void Collision(int, float);
	void Draw(sf::Vector2f, sf::Vector2f, float);
	void Reload();
	void SetDrawRange(float);
	void SetRate(float);
	std::pair<int, int> GetAmmo();
	~Gun();
};


#endif