#pragma once
#include "pch.h"
#include "Subsystem.h"

namespace Grengine {
    class AudioSystem : public Subsystem {
    public:
        virtual void play(std::string name)                               = 0;
       // virtual void stop(std::string name)                               = 0;
       // virtual void stopAll()                                            = 0;
        virtual void loadSound(std::string name, std::string filename)    = 0;
    };

    extern AudioSystem* grAudioSystem;
}


