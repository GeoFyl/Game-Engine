#include "pch.h"
#include "WindowSystem_SDL.h"
#include "SystemsLocator.h"
//#include <SDL.h>
#include "SDL.h"
#include "SDL_syswm.h"

using namespace Toffee::Internal;

WindowSystem_SDL* window_SDL = new WindowSystem_SDL;

WindowSystem_SDL::WindowSystem_SDL() {
	ProvideSystem(this);
	window_SDL = nullptr;
}

// Initialise window flags
int WindowSystem_SDL::Initialise() {
    flags_ = SDL_WindowFlags::SDL_WINDOW_SHOWN | SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI | SDL_WindowFlags::SDL_WINDOW_RESIZABLE;
    int result = 0;

    try {
        //Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) throw;
        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY) < 0) throw;
        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1) < 0) throw;
        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1) < 0) throw;
        if (SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8) < 0) throw;
        flags_ |= SDL_WindowFlags::SDL_WINDOW_OPENGL;
    }
    catch (...) {
        printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
        result = -1;
    }

    return result;
}

int WindowSystem_SDL::Shutdown() {
    SDL_DestroyWindow(window_);
	return 0;
}

// Open the window. Set name and dimensions.
int WindowSystem_SDL::OpenWindow(std::string name, int width, int height) {
    int result = 0;

    screenWidth_ = width;
    screenHeight_ = height;

    // Open the window
    window_ = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth_, screenHeight_, flags_);
    if (window_ == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        result = -1;
    }

    // Make the rendering system create the renderer
    CreateRenderer();

    return result;
}

// Handle events
void WindowSystem_SDL::HandleWindowEvent(WindowEvent& e) {
    switch (e.event) {
    case WindowEventID::WINDOWEVENT_SHOWN:
        SDL_Log("Window %d shown", e.windowID);
        break;
    case WindowEventID::WINDOWEVENT_HIDDEN:
        SDL_Log("Window %d hidden", e.windowID);
        break;
    case WindowEventID::WINDOWEVENT_EXPOSED:
        SDL_Log("Window %d exposed", e.windowID);
        break;
    case WindowEventID::WINDOWEVENT_MOVED:
        SDL_Log("Window %d moved to %d,%d",
            e.windowID, e.data1,
            e.data2);
        break;
    case WindowEventID::WINDOWEVENT_RESIZED:
        SDL_Log("Window %d resized to %dx%d",
            e.windowID, e.data1,
            e.data2);
        Resize(e.data1, e.data2); // Make the rendering system resize the viewport
        break;
    case WindowEventID::WINDOWEVENT_SIZE_CHANGED:
        SDL_Log("Window %d size changed to %dx%d",
            e.windowID, e.data1,
            e.data2);
        Resize(e.data1, e.data2); // Make the rendering system resize the viewport
        break;
    case WindowEventID::WINDOWEVENT_MINIMIZED:
        SDL_Log("Window %d minimized", e.windowID);
        break;
    case WindowEventID::WINDOWEVENT_MAXIMIZED:
        SDL_Log("Window %d maximized", e.windowID);
        break;
    case WindowEventID::WINDOWEVENT_RESTORED:
        SDL_Log("Window %d restored", e.windowID);
        break;
    case WindowEventID::WINDOWEVENT_ENTER:
        SDL_Log("Mouse entered window %d",
            e.windowID);
        break;
    case WindowEventID::WINDOWEVENT_LEAVE:
        SDL_Log("Mouse left window %d", e.windowID);
        break;
    case WindowEventID::WINDOWEVENT_FOCUS_GAINED:
        SDL_Log("Window %d gained keyboard focus",
            e.windowID);
        break;
    case WindowEventID::WINDOWEVENT_FOCUS_LOST:
        SDL_Log("Window %d lost keyboard focus",
            e.windowID);
        break;
    case WindowEventID::WINDOWEVENT_CLOSE:
        SDL_Log("Window %d closed", e.windowID);
        break;

    default:
        SDL_Log("Window %d got unknown event %d",
            e.windowID, e.event);
        break;
    }
}

// Get data relating to the window. 
// Returns HWND and device context but can be changed for other operating systems
WindowInfo WindowSystem_SDL::GetWindowInfo() const
{
    WindowInfo window_info;

#ifdef _WIN32||_WIN64
    SDL_SysWMinfo SDL_window_info;
    SDL_VERSION(&SDL_window_info.version);
    if (!SDL_GetWindowWMInfo(window_, &SDL_window_info)) {
        SDL_Log(SDL_GetError());
    };
    window_info.handle = SDL_window_info.info.win.window;
    window_info.device = SDL_window_info.info.win.hdc;
#endif

    return window_info;
}

