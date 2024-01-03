#pragma once
#include "EventSystem.h"
#include <SDL.h>

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

    protected:
        #ifdef _WIN32||_WIN64
        HWND hwnd_;
        #else
        int msg_;
        #endif
    };
}