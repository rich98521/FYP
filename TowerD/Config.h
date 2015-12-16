#ifndef CONFIG_H
#define CONFIG_H
using namespace std;
#include<iostream>
#include<fstream>
#include<map>

//enum for render layers
const static enum Layer {
	TILE, ENTITYBASE, CHARACTERBASE, GHOST, BULLET, CHARACTERTOP, ENTITYTOP, ENTITYHIGH, HUD, UI, CROSSHAIRS
};

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
