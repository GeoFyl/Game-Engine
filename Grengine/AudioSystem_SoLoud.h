#pragma once
#include "pch.h"
#include "AudioSystem.h"
#include "soloud.h"

namespace Grengine {
    class AudioSystem_SoLoud : public AudioSystem {
    public:
        int Initialise();
        int Shutdown();
        void play(std::string name);
       // void stop(std::string name);
       // void stopAll();
        void loadSound(std::string name, std::string filename);

    private:
        SoLoud::Soloud soLoud;
        std::unordered_map<std::string, SoLoud::AudioSource> audioSources;
    };
}


