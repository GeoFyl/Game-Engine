#pragma once
#include "AudioSystem.h"
#include <soloud_wav.h>
#include <unordered_map>

namespace Toffee::Internal {
    class AudioSystem_SoLoud : public AudioSystem {
    public:
        AudioSystem_SoLoud();

    private:
        // Inherited from Subsystem

        int Initialise() final;
        int Shutdown() final;

        // See AudioSystem.h for function details

        void Play(std::string name) final;
        void Stop(std::string name) final;
        void StopAll() final;

        SoLoud::Soloud soLoud_;
        std::unordered_map<std::string, SoLoud::handle> active_sounds_;
    };
}