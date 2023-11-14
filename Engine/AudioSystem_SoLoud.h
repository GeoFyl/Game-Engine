#pragma once
//#include "pch.h"
#include "AudioSystem.h"
#include "soloud.h"
#include "Core.h"

namespace Engine::Internal {
    class AudioSystem_SoLoud : public AudioSystem {
    public:

        AudioSystem_SoLoud();
        int Initialise() final;
        int Shutdown() final;

        void Play(std::string name);
       // void stop(std::string name);
       // void stopAll();
        void LoadSound(std::string name, std::string filename);

    private:

        SoLoud::Soloud soLoud;
        std::unordered_map<std::string, SoLoud::AudioSource*> audioSources;
    };
}