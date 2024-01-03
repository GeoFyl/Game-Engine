#pragma once
#include "Subsystem.h"
#include "InputSystem.h"
#include "WindowEvents.h"


namespace Engine::Internal {
    /// <summary>
    /// Interface base class to event system
    /// </summary>
    class EventSystem : public Subsystem {
    public:
        EventSystem() { type_ = SubsytemType::EVENT; }
        virtual int ProcessEvents() = 0;

    protected:
        void HandleInputEvent(void* msg);
        void HandleWindowEvent(WindowEvent& e);
    };
}
