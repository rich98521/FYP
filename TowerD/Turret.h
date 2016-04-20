#ifndef TURRET_H
#define TURRET_H
#include "Enemy.h"	
#include "Gun.h"	
#include <array>

class Turret : public Entity
{
private:
	Gun mGun;
	std::vector<std::pair<sf::Vector2i, float>> mPath;
	std::vector<Enemy*>* mEnemies;
	bool mShooting = false;
	float mRange = 64;
	int mType = 0;
	int mCost = 15;
	std::array<std::array<float, 3>, 5> mUpgradeTable;
	std::array<int, 5> mUpgradeCosts;
	int mLevel = 0;
	float mBaseDamage, mBaseRate, mBaseRange;
	int missileAimDelay = 0;
	Sprite* mHighlightSprite;
	bool mHighlighted;

public:
	Turret(sf::Vector2f, int, int, Renderer*, std::vector<Enemy*>*);
	void Draw(sf::Vector2f, float);
	void Update(float, sf::Vector2f, float);
	void LoadAssets();
	void SetId(int);
	sf::Vector2f Shoot();
	int GetCost(){ return mCost; }
	int GetType(){ return mType; }
	Gun* GetGun();
	void SetLevel(int);
	int GetLevel();
	int GetUpgradeCost(int l){ return mUpgradeCosts[l]; };
	void Highlight(bool b){ mHighlightSprite->SetVisible(b); }

};


#endif