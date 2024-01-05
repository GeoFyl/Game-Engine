#pragma once
#include "EventSystem.h"
#include <SDL.h>

namespace Toffee::Internal {
    class EventSystem_SDL : public EventSystem {
    public:
        EventSystem_SDL();
    protected:
        int Initialise() final;
        int Shutdown() final;
        int ProcessEvents() final;
    };
}