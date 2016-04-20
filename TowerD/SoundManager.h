#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "SFML/Graphics.hpp"
#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

class SoundManager
{
public:
	SoundManager();
	void Init();
	static void PlaySoundEffect(string);
	static void PlayMusic(string);
	static void StopMusic();
	static string mPlayingSong;
	static void SetMusicVolume(int);
	static void SetEffectVolume(int);

private:
	static std::map<string, sf::Music*> mMusics;
	static std::map<string, sf::Sound*> mEffects;
	static int mMusicVolume, mEffectVolume;
	std::vector<sf::SoundBuffer*> mBuffers;

};

#endif // !SOUNDMANAGER_H
