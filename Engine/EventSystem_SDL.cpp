#include "pch.h"
#include "EventSystem_SDL.h"
#include "SystemsLocator.h"
#include <SDL.h>

using namespace Engine::Internal;
//We're globally instancing our subsystems!
//But maybe you think Globals are bad
//There might be other ways to approach this.
//#ifdef RENDERSYSTEM_SDL
//    EventSystem_SDL grEventSystemSDL;
//    EventSystem* event_system_ = &grEventSystemSDL;
//#endif
EventSystem_SDL* event_SDL = new EventSystem_SDL;

Engine::Internal::EventSystem_SDL::EventSystem_SDL() {
    ProvideSystem(this);
    event_SDL = nullptr;
}

int EventSystem_SDL::Initialise()
{
    return 0;
}

int EventSystem_SDL::Shutdown()
{
    return 0;
}

int EventSystem_SDL::ProcessEvents()
{
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
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                returnValue = -1;
            }
            break;
        case SDL_KEYUP:

            break;
        case SDL_WINDOWEVENT:
            //Here we're converting the SDL event into our generic Grengine event
            //so that we can keep things nice and abstract
            
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
            SystemsAPI::Window()->HandleWindowEvent(we);
        break;
    }
}
return returnValue;
}
