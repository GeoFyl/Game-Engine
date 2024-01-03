#pragma once
#include "AudioSystem.h"
#include <soloud_wav.h>

namespace Engine::Internal {
    class AudioSystem_SoLoud : public AudioSystem {
    public:
        AudioSystem_SoLoud();
        int Initialise() final;
        int Shutdown() final;

        void Play(std::string name);
       // void stop(std::string name);
       // void stopAll();
        //void LoadSound(std::string name, std::string filename);

    private:

        SoLoud::Soloud soLoud_;
        //std::unordered_map<std::string, SoLoud::AudioSource*> audioSources_;
    };
}