#include "Config.h"
#include "stdafx.h"

//loads in screen resolution from config file
void Config::Init()
{
	ifstream myReadFile;
	string line;
	myReadFile.open("../Config.txt");

	getline(myReadFile, line);
	mScreenWidth = atoi(line.c_str());
	getline(myReadFile, line);
	mScreenHeight = atoi(line.c_str());

	myReadFile.close();
}

int Config::ScreenWidth()
{
	return mScreenWidth;
}

int Config::ScreenHeight()
{
	return mScreenHeight;
}

sf::Packet& operator <<(sf::Packet& packet, const PlayerNamePacket& ent)
{
	return packet << (byte)UPDATENAME << ent.name;
}

sf::Packet& operator >>(sf::Packet& packet, PlayerNamePacket& ent)
{
	return packet >> ent.name;
}


sf::Packet& operator <<(sf::Packet& packet, const DisconnectPacket& ent)
{
	return packet << (byte)DISCONNECT;
}

sf::Packet& operator >>(sf::Packet& packet, DisconnectPacket& ent)
{
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const WaveStartPacket& ent)
{
	return packet << (byte)WAVESTART << ent.enemyCount << ent.waveCount << ent.currentWave;
}

sf::Packet& operator >>(sf::Packet& packet, WaveStartPacket& ent)
{
	return packet >> ent.enemyCount >> ent.waveCount >> ent.currentWave;
}

sf::Packet& operator <<(sf::Packet& packet, const TurretPacket& ent)
{
	return packet << (byte)NEWTURRET << ent.xPos << ent.yPos << ent.type;
}

sf::Packet& operator >>(sf::Packet& packet, TurretPacket& ent)
{
	return packet >> ent.xPos >> ent.yPos >> ent.type;
}

sf::Packet& operator <<(sf::Packet& packet, const GrenadePacket& ent)
{
	return packet << (byte)NEWGRENADE << ent.xVel << ent.yVel << ent.xPos << ent.yPos << ent.height;
}

sf::Packet& operator >>(sf::Packet& packet, GrenadePacket& ent)
{
	return packet >> ent.xVel >> ent.yVel >> ent.xPos >> ent.yPos >> ent.height;
}

sf::Packet& operator <<(sf::Packet& packet, const GunShotPacket& ent)
{
	return packet << (byte)GUNSHOT << ent.id << ent.angle;
}

sf::Packet& operator >>(sf::Packet& packet, GunShotPacket& ent)
{
	return packet >> ent.id >> ent.angle;
}

sf::Packet& operator <<(sf::Packet& packet, const EntityMovePacket& ent)
{
	return packet << (byte)ENTITYMOVE << ent.id << ent.xVel << ent.yVel << ent.xAcc << ent.yAcc << ent.xPos << ent.yPos << ent.zPos;
}

sf::Packet& operator >>(sf::Packet& packet, EntityMovePacket& ent)
{
	return packet >> ent.id >> ent.xVel >> ent.yVel >> ent.xAcc >> ent.yAcc >> ent.xPos >> ent.yPos >> ent.zPos;
}

sf::Packet& operator <<(sf::Packet& packet, const NewEntityPacket& ent)
{
	return packet << (byte)NEWENTITY << ent.id << ent.xPos << ent.yPos << ent.type << ent.t1 << ent.t2 << ent.t3;
}

sf::Packet& operator >>(sf::Packet& packet, NewEntityPacket& ent)
{
	return packet >> ent.id >> ent.xPos >> ent.yPos >> ent.type >> ent.t1 >> ent.t2 >> ent.t3;
}

sf::Packet& operator <<(sf::Packet& packet, const NewTilePacket& ent)
{
	return packet << (byte)NEWTILE << ent.x << ent.y << ent.type;
}

sf::Packet& operator >>(sf::Packet& packet, NewTilePacket& ent)
{
	return packet >> ent.x >> ent.y >> ent.type;
}

sf::Packet& operator <<(sf::Packet& packet, const EntityDamagePacket& ent)
{
	return packet << (byte)ENTITYDAMAGE << ent.id << ent.health;
}

sf::Packet& operator >>(sf::Packet& packet, EntityDamagePacket& ent)
{
	return packet >> ent.id >> ent.health;
}

sf::Packet& operator <<(sf::Packet& packet, const LoadMapPacket& ent)
{
	return packet << (byte)LOADMAP << ent.number;
}

sf::Packet& operator >>(sf::Packet& packet, LoadMapPacket& ent)
{
	return packet >> ent.number;
}

sf::Packet& operator <<(sf::Packet& packet, const EntityRotatePacket& ent)
{
	return packet << (byte)ENTITYROTATE << ent.id << ent.ang << ent.baseAng;
}

sf::Packet& operator >>(sf::Packet& packet, EntityRotatePacket& ent)
{
	return packet >> ent.id >> ent.ang >> ent.baseAng;
}

int Config::mScreenWidth;
int Config::mScreenHeight;