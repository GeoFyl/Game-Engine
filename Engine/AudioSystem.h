#pragma once
#include "pch.h"
#include "Subsystem.h"

namespace Toffee::Internal {
    // Interface base class to audio system
    class AudioSystem : public Subsystem {
    public:
        AudioSystem() { type_ = SubsytemType::AUDIO; }
        // Play an audio source
        virtual void Play(std::string source) = 0;
        // Stop an audio source
        virtual void Stop(std::string source) = 0;
        // Stop all audio
        virtual void StopAll() = 0;
    };
}


