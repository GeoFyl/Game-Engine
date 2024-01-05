#pragma once
#include "Subsystem.h"
#include "InputSystem.h"
#include "WindowEvents.h"


namespace Toffee::Internal {
    /// <summary>
    /// Interface base class to event system
    /// </summary>
    class EventSystem : public Subsystem {
    public:
        EventSystem() { type_ = SubsytemType::EVENT; }

    protected:
        friend class Core;
        // Process events
        virtual int ProcessEvents() = 0;
        // Sends data to input system
        void HandleInputEvent(void* msg);
        // Sends data to window system
        void HandleWindowEvent(WindowEvent& e);
    };
}
