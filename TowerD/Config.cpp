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

int Config::mScreenWidth;
int Config::mScreenHeight;