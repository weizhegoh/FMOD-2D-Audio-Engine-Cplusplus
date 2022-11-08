/*****************************************************************
*\file		 AudioEngine.cpp
*\brief		 Contains definition for audio engine

*\author(s)  Goh Wei Zhe	153 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "AudioEngine.h"

Audio* audio = nullptr;
AudioEngine* audio_Engine;

Audio::Audio() :Syst(nullptr)
{
    audio_Engine->ErrorCheck(FMOD::System_Create(&Syst));
    audio_Engine->ErrorCheck(Syst->init(32, FMOD_INIT_NORMAL, NULL));
}

void Audio::Update()
{
    std::vector<ChannelMap::iterator> stoppedChannel;

    if (channelMap.empty())
    {
        std::cout << "map is empty" << std::endl;
        std::cout << "soundMap.size(): " << soundMap.size() << std::endl;
        std::cout << "channelMap.size(): " << channelMap.size() << std::endl;
        return;
    }
    else
    {
        std::cout << "soundMap.size(): " << soundMap.size() << std::endl;
        std::cout << "channelMap.size(): " << channelMap.size() << std::endl;


        for (auto it = channelMap.begin(), itEnd = channelMap.end(); it != itEnd; ++it)
        {
            bool playing = false;
            it->second->isPlaying(&playing);

            if (!playing)
            {
                stoppedChannel.push_back(it);
            }
        }
    }

    for (auto& it : stoppedChannel)
    {
        channelMap.erase(it);
    }

    audio_Engine->ErrorCheck(Syst->update());
}

Audio::~Audio()
{
    audio_Engine->ErrorCheck(Syst->release());
}

void AudioEngine::LoadSound(const std::string& audioTrack, FMOD_MODE loop)
{
    auto it = audio->soundMap.find(audioTrack);

    //Audio exist
    if (it != audio->soundMap.end())
    {
        std::cout << "Audio Track Exist" << std::endl;
        return;
    }

    FMOD::Sound* sound = nullptr;

    AudioEngine::ErrorCheck(audio->Syst->createSound(audioTrack.c_str(), loop, nullptr, &sound));

    if (sound)
        audio->soundMap[audioTrack] = sound;
}

void AudioEngine::PlaySound(const std::string& audioTrack, FMOD_MODE loop, float volume)
{
    int channelID = audio->channelCount++;

    if (audio->soundMap.empty())
    {
        std::cout << "Sound Map is empty" << std::endl;

        LoadSound(audioTrack, loop);
    }

    if (!audio->soundMap.empty())
    {
        auto it = audio->soundMap.find(audioTrack);

        if (it == audio->soundMap.end())
            LoadSound(audioTrack, loop);
    }

    auto it = audio->soundMap.find(audioTrack);

    FMOD::Channel* channel = nullptr;
    AudioEngine::ErrorCheck(audio->Syst->playSound(it->second, nullptr, true, &channel));

    if (channel)
    {
        AudioEngine::ErrorCheck(channel->setVolume(volume));
        AudioEngine::ErrorCheck(channel->setPaused(false));
        audio->channelMap[channelID] = channel;
    }
}

void AudioEngine::UnloadSound(const std::string& audioTrack)
{
    auto it = audio->soundMap.find(audioTrack);

    if (it == audio->soundMap.end())
        return;

    AudioEngine::ErrorCheck(it->second->release());
    audio->soundMap.erase(it);
}

int AudioEngine::ErrorCheck(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        std::cout << "FMOD ERROR " << result << std::endl;
        return 1;
    }
    return 0;
}

void AudioEngine::Init()
{
    audio = new Audio();
}

void AudioEngine::Update()
{
    CheckInput();
    audio->Update();
}

void AudioEngine::Shutdown()
{
    std::cout << "soundMap.size(): " << audio->soundMap.size() << std::endl;
    std::cout << "channelMap.size(): " << audio->channelMap.size() << std::endl;
    delete audio;
}