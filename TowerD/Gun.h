#ifndef GUN_H
#define GUN_H
#include "Sprite.h" 
#include "Renderer.h" 
#include "Network.h" 
#include "SoundManager.h" 


class Gun
{
private:
	float mAccuracy = 0.98f;
	//float mAccuracy = 1;
	float mDamage = 1;
	float mRate = 0.15f;
	int mMag;
	int mMagTotal = 20;
	int mReloadRate = 1;
	float mRange = 480, mDrawRange = 480;
	int mBulletCount = 1;
	int mType = 0;
	sf::Clock mClock;
	sf::Clock mReloadClock;
	Renderer* mRen;
	bool mShot;
	std::vector<Sprite*> mBulletSprites;
	std::vector<sf::Vector2f*> mBulletHitPos;
	sf::Color mBulletColor;
	int mOwnerId;

public:
	Gun() 
	{
	}
	Gun(Renderer*, int, int);
	void LoadAssets();
	std::vector<sf::Vector2f*> Shoot(sf::Vector2f, float);
	std::vector<sf::Vector2f*> GetBullets();
	float GetDamage();
	float GetRange();
	void SetDamage(float);
	void Collision(int, float);
	void Draw(sf::Vector2f, sf::Vector2f, float);
	void Reload();
	void SetDrawRange(float);
	void SetRate(float);
	void SetRange(float);
	void SetAccuracy(float);
	void SetMaxAmmo(int);
	void SetColor(sf::Color);
	void SetBulletCount(int);
	int GetOwnerId(){ return mOwnerId; }
	void SetOwnerId(int id){ mOwnerId = id; }
	void ClearBullets(){ mBulletHitPos.clear(); }
	std::pair<int, int> GetAmmo();
	~Gun();
};


#endif