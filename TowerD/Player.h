#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"	
#include "Gun.h"	
#include "SoundManager.h"
#include <array>

class Player : public Entity
{
private:
	sf::Vector2i mAimLoc, dir, mouseLast, mPlacePos;
	void Aim(sf::Vector2f, float);
	Sprite* mCrosshairs;
	Sprite* mBaseSprite2;
	Sprite* mSprite2;
	Sprite* mSprite3;
	float mBaseAngle2;
	std::vector<Sprite*> mGhosts;
	int mEquipped;
	bool mLDown, mRDown;
	Gun mGun;
	bool invuln = false;
	bool mVisible = true;
	sf::Clock mInvulnTimer;
	sf::Vector2f mSpawnPos;
	bool mJetpack = false, mJetPackEmptied = false;
	float mJetFuel;
	float mJetFuelMax = 300;
	float jetAnmFrame = 0;
	int mCredits = 200;
	float mSpeed;
	std::array<float, 5> mAccTable;
	std::array<float, 5> mDamTable;
	std::array<float, 5> mRateTable;
	std::array<int, 5> mAmmoTable;
	std::array<int, 5> mUpgradeCosts;
	std::array<float, 7> mLevels;

public:
	Player(sf::Vector2f, int, Renderer*);
	void ProcessInput(sf::Event, sf::Vector2f, float);
	void Update(float, sf::Vector2f, float);
	void Draw(sf::Vector2f, float);
	void LoadAssets();
	sf::Vector2i* AimPos();
	sf::Vector2f* Pos();
	std::pair<int, sf::Vector2i> GetPlace();
	std::pair<int, sf::Vector2i> GetRemove();
	Gun* GetGun();
	bool Hit(float);
	void SetVisible(bool);
	bool Alive();
	void Respawn();
	void Shoot();
	float JetFuel();
	void AddCredits(int);
	int GetCredits() { return mCredits; };
	float GetMaxHealth(){ return mMaxHealth; };
	float GetMaxFuel(){ return mJetFuelMax; };
	void SetAccuracyLevel(int l) { mGun.SetAccuracy(mAccTable[l]); mLevels[0] = l; };
	void SetDamageLevel(int l) { mGun.SetDamage(mDamTable[l]);  mLevels[1] = l; };
	void SetRateLevel(int l) { mGun.SetRate(mRateTable[l]);  mLevels[2] = l; };
	void SetAmmoLevel(int l) { mGun.SetMaxAmmo(mAmmoTable[l]);  mLevels[3] = l; };
	void SetHealthLevel(int l) { mMaxHealth = 10 + 2 * l; mLevels[4] = l; };
	void SetFuelLevel(int l) { mJetFuelMax = 250 + 50 * l; mLevels[5] = l; };
	void SetSpeedLevel(int l) { mSpeed = 800 + 80 * l;  mLevels[6] = l; };
	int GetAccuracyLevel() { return mLevels[0]; };
	int GetDamageLevel() { return mLevels[1]; };
	int GetRateLevel() { return mLevels[2]; };
	int GetAmmoLevel() { return mLevels[3]; };
	int GetHealthLevel() { return mLevels[4]; };
	int GetFuelLevel() { return mLevels[5]; };
	int GetSpeedLevel() { return mLevels[6]; };
	int GetUpgradeCost(int l){ return mUpgradeCosts[l]; };

	~Player();
};


#endif