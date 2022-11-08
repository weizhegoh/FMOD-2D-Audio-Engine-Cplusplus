/*****************************************************************
*\file		 AudioEngine.h
*\brief		 Contains declaration for audio engine

*\author(s)  Goh Wei Zhe	53 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include "fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <iostream>

struct Audio
{
	Audio();
	void Update();
	~Audio();

	int channelCount = 0;

	FMOD::System* Syst;

	typedef std::map<std::string, FMOD::Sound*> SoundMap;
	typedef std::map<int, FMOD::Channel*> ChannelMap;

	SoundMap soundMap;
	ChannelMap channelMap;
};

class AudioEngine
{
public:
	float volume = 1.0f;

	void LoadSound(const std::string& audioTrack, FMOD_MODE loop);
	void PlaySound(const std::string& audioTrack, FMOD_MODE loop, float volume = 1.0f);
	void UnloadSound(const std::string& audioTrack);
	int ErrorCheck(FMOD_RESULT result);

	void Init();
	void Update();
	void Shutdown();
};

#endif