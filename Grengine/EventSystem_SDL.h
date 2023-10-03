#pragma once
#include "EventSystem.h"

namespace Grengine {
    /// <summary>
    /// SDL Implementation to our EventSystem interface
    /// Handles the SDL message loop and dispatches
    /// the messages
    /// </summary>
    class EventSystem_SDL :
        public EventSystem
    {
    public:
        int Initialise();
        int Shutdown();
    public:
        int ProcessEvents() final;
    };
}