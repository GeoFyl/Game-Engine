#pragma once
#include "EventSystem.h"

namespace Engine::Internal {
    /// <summary>
    /// SDL Implementation to our EventSystem interface
    /// Handles the SDL message loop and dispatches
    /// the messages
    /// </summary>
    class EventSystem_SDL : public EventSystem {
    public:
        EventSystem_SDL();
        int Initialise();
        int Shutdown();
        int ProcessEvents() final;
    };
}