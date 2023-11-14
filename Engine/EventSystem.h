#pragma once
#include "Subsystem.h"
#include "WindowEvents.h"

namespace Engine::Internal {
    /// <summary>
    /// Interface base class to our event system
    /// </summary>
    class EventSystem : public Subsystem {
    public:
        EventSystem() { type_ = SubsytemType::EVENT; }
        virtual int ProcessEvents() = 0;
    };
}
