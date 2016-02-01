#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"	
#include "Gun.h"	
#include "SoundManager.h"

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
	bool mDown;
	Gun mGun;
	bool invuln = false;
	bool mVisible = true;
	sf::Clock mInvulnTimer;
	sf::Vector2f mSpawnPos;
	bool mJetpack = false, mJetPackEmptied = false;
	float mJetFuel;
	float mJetFuelMax = 300;
	float jetAnmFrame = 0;

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
	~Player();
};


#endif