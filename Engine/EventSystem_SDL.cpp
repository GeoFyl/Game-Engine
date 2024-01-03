#include "pch.h"
#include "EventSystem_SDL.h"
#include "SystemsLocator.h"


using namespace Engine::Internal;

EventSystem_SDL* event_SDL = new EventSystem_SDL;

Engine::Internal::EventSystem_SDL::EventSystem_SDL() {
    ProvideSystem(this);
    event_SDL = nullptr;
}

int EventSystem_SDL::Initialise() {
    return 0;
}

int EventSystem_SDL::Shutdown() {
    return 0;
}

int EventSystem_SDL::ProcessEvents() {

#ifdef _WIN32||_WIN64
    MSG msg;
    while (PeekMessage(&msg, reinterpret_cast<HWND>(SystemsAPI::Window()->GetWindowInfo().handle), WM_KEYFIRST, 0, PM_REMOVE | PM_QS_INPUT)) {
            HandleInputEvent(&msg);
    }
#endif

    SDL_Event e;
    //While application is running
    int returnValue = 0;
  
    //Standard SDL event system - go look at their documentation if you care!
    //Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
        case SDL_QUIT:
            //User requests quit
            returnValue = -1;
            break;
        case SDL_WINDOWEVENT:
            //Abstract the SDL event into generic event
            WindowEvent we;
            {
                we.event = static_cast<WindowEventID>(e.window.event);
                we.data1 = e.window.data1;
                we.data2 = e.window.data2;
                we.padding1 = e.window.padding1;
                we.padding2 = e.window.padding2;
                we.padding3 = e.window.padding3;
                we.timestamp = e.window.timestamp;
                we.type = e.window.type;
                we.windowID = e.window.windowID;
            }
            HandleWindowEvent(we);
        break;
    }
}
return returnValue;
}