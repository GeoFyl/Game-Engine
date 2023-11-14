#pragma once
#include "pch.h"
#include "Subsystem.h"

namespace Engine::Internal {
    class AudioSystem : public Subsystem {
    public:
        AudioSystem() { type_ = SubsytemType::AUDIO; }
        virtual void Play(std::string name) = 0;
        // virtual void stop(std::string name)                               = 0;
        // virtual void stopAll()                                            = 0;
        virtual void LoadSound(std::string name, std::string filename) = 0;
    };
}


