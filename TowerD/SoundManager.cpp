#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
}

void SoundManager::Init()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	string last;
	hFind = FindFirstFile("../Audio/Effects/*", &FindFileData);
	FindNextFile(hFind, &FindFileData);
	FindNextFile(hFind, &FindFileData);
	while (FindFileData.cFileName != last)
	{
		std::cout << FindFileData.cFileName << std::endl;
		last = FindFileData.cFileName;
		sf::SoundBuffer* buff = new sf::SoundBuffer();
		buff->loadFromFile("../Audio/Effects/" + last);
		mBuffers.push_back(buff);
		string key = last.substr(0, last.find_first_of('.'));
		mEffects[key] = new sf::Sound();
		mEffects[key]->setBuffer(*mBuffers.back());
		//mMusics[last] = new sf::Sound();
		//mMusics.end()->second->("../Audio/Music/" + last);
		FindNextFile(hFind, &FindFileData);
	}
	FindClose(hFind);
	hFind = FindFirstFile("../Audio/Music/*", &FindFileData);
	FindNextFile(hFind, &FindFileData);
	FindNextFile(hFind, &FindFileData);
	while (FindFileData.cFileName != last)
	{
		std::cout << FindFileData.cFileName << std::endl;
		last = FindFileData.cFileName;
		string key = last.substr(0, last.find_first_of('.'));
		mMusics[key] = new sf::Music();
		mMusics[key]->openFromFile("../Audio/Music/" + last);
		mMusics[key]->setLoop(true);
		FindNextFile(hFind, &FindFileData);
	}
	FindClose(hFind);
}

void SoundManager::PlaySoundEffect(string s)
{
	mEffects[s]->play();
}

void SoundManager::PlayMusic(string s)
{
	StopMusic();
	mMusics[s]->play();
	mPlayingSong = s;
}

void SoundManager::StopMusic()
{
	if (mPlayingSong != "")
		mMusics[mPlayingSong]->stop();
}

std::map<std::string, sf::Music*> SoundManager::mMusics;
std::map<std::string, sf::Sound*> SoundManager::mEffects;
string SoundManager::mPlayingSong;