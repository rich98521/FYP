#ifndef CONFIG_H
#define CONFIG_H
using namespace std;
#include<iostream>
#include<fstream>
#include<map>
#include <SFML/Network.hpp>

//enum for render layers
const static enum Layer {
	TILE, ENTITYBASE, CHARACTERBASE, GHOST, BULLET, CHARACTERTOP, ENTITYTOP, ENTITYHIGH, HUD, UI, CROSSHAIRS
};
const static enum Controller {
	A, B, X, Y, LB, RB, BACK, START, LA, RA
};
const static enum PacketType {
	ENTITYMOVE, NEWENTITY, NEWTILE, ENTITYDAMAGE, ENTITYROTATE, LOADMAP, NEWGRENADE, NEWTURRET, NEWMISSILE, GUNSHOT, WAVESTART
};
const static enum EntityType {
	CORE, ENEMY, EXPLOSION, GRENADE, MISSILE, PLAYER, TURRET
};
const static enum ModifierKeys {
	None = 0, Control = 1, Shift = 2, Alt = 4
};
inline ModifierKeys operator|(ModifierKeys a, ModifierKeys b)
{
	return static_cast<ModifierKeys>(static_cast<int>(a) | static_cast<int>(b));
}

struct EntityMovePacket
{
	int id = -1;
	float xVel;
	float yVel;
	float xAcc;
	float yAcc;
	float xPos;
	float yPos;
	float zPos;

};

sf::Packet& operator <<(sf::Packet& packet, const EntityMovePacket& ent);

sf::Packet& operator >>(sf::Packet& packet, EntityMovePacket& ent);

struct NewEntityPacket
{
	int id = -1;
	int xPos;
	int yPos;
	byte type;
	float t1, t2, t3;
};

sf::Packet& operator <<(sf::Packet& packet, const NewEntityPacket& ent);

sf::Packet& operator >>(sf::Packet& packet, NewEntityPacket& ent);

struct NewTilePacket
{
	int x;
	int y;
	byte type;
};

sf::Packet& operator <<(sf::Packet& packet, const NewTilePacket& ent);

sf::Packet& operator >>(sf::Packet& packet, NewTilePacket& ent);

struct WaveStartPacket
{
};

sf::Packet& operator <<(sf::Packet& packet, const WaveStartPacket& ent);

sf::Packet& operator >>(sf::Packet& packet, WaveStartPacket& ent);

struct EntityDamagePacket
{
	int id;
	byte health;
};

sf::Packet& operator <<(sf::Packet& packet, const EntityDamagePacket& ent);

sf::Packet& operator >>(sf::Packet& packet, EntityDamagePacket& ent);

struct LoadMapPacket
{
	int number;
};

sf::Packet& operator <<(sf::Packet& packet, const LoadMapPacket& ent);

sf::Packet& operator >>(sf::Packet& packet, LoadMapPacket& ent);

struct EntityRotatePacket
{
	int id;
	float ang;
	float baseAng;
};

sf::Packet& operator <<(sf::Packet& packet, const EntityRotatePacket& ent);

sf::Packet& operator >>(sf::Packet& packet, EntityRotatePacket& ent);

struct GrenadePacket
{
	float xVel;
	float yVel;
	float xPos;
	float yPos;
	float height;
};

sf::Packet& operator <<(sf::Packet& packet, const GrenadePacket& ent);

sf::Packet& operator >>(sf::Packet& packet, GrenadePacket& ent);

struct GunShotPacket
{
	int id;
	float angle;
};

sf::Packet& operator <<(sf::Packet& packet, const GunShotPacket& ent);

sf::Packet& operator >>(sf::Packet& packet, GunShotPacket& ent);

struct TurretPacket
{
	float xPos;
	float yPos;
	int type;
};

sf::Packet& operator <<(sf::Packet& packet, const TurretPacket& ent);

sf::Packet& operator >>(sf::Packet& packet, TurretPacket& ent);

struct FullMapPacket
{
	std::vector<byte> map;
	int width;
	int height;
};

sf::Packet& operator <<(sf::Packet& packet, const FullMapPacket& map)
{
	packet << -1;
	packet << map.width;
	packet << map.height;
	for (int y = 0; y < map.height; y++)
		for (int x = 0; x < map.width; x++)
			packet << map.map[y*map.height + x];
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, FullMapPacket& map)
{
	packet >> map.width;
	packet >> map.height;
	byte id = 0;
	for (int i = 0; i < map.height*map.width; i++)
	{
		packet >> id;
		map.map.push_back(id);
	}
	return packet;
}

class Config
{
	static int mScreenWidth;
	static int mScreenHeight;
public:
	static void Init();
	static int ScreenWidth();
	static int ScreenHeight();
};

#endif
