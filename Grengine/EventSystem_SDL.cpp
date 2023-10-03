#include "pch.h"
#include "EventSystem_SDL.h"
#include "Core.h"  
#include "RenderSystem.h"
#include <SDL.h>

//We're globally instancing our subsystems!
//But maybe you think Globals are bad
//There might be other ways to approach this.
#ifdef RENDERSYSTEM_SDL
    Grengine::EventSystem_SDL grEventSystemSDL;
    Grengine::EventSystem* Grengine::grEventSystem = &grEventSystemSDL;
#endif

int Grengine::EventSystem_SDL::Initialise()
{
    return 0;
}

int Grengine::EventSystem_SDL::Shutdown()
{
    return 0;
}

int Grengine::EventSystem_SDL::ProcessEvents()
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
            GR_WindowEvent we;
            {                
                we.event = static_cast<RenderSystem::GR_WINDOWEVENT>(e.window.event);
                we.data1 = e.window.data1;
                we.data2 = e.window.data2;
                we.padding1 = e.window.padding1;
                we.padding2 = e.window.padding2;
                we.padding3 = e.window.padding3;
                we.timestamp = e.window.timestamp;
                we.type = e.window.type;
                we.windowID = e.window.windowID;
            }
            Grengine::grRenderSystem->HandleWindowEvent(we);
            break;
        }
    }
    return returnValue;
}
