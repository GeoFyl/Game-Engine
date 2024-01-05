#pragma once
#include "WindowSystem.h"

class SDL_Window;

namespace Toffee::Internal {
	class WindowSystem_SDL : public WindowSystem {
	public:
		WindowSystem_SDL();

	private:

		//Inherited from WindowSystem interface

		int	OpenWindow(std::string name, int width, int height) final;
		void HandleWindowEvent(WindowEvent& e) final;
		int	GetWidth() const final { return screenWidth_; };
		int	GetHeight() const final { return screenHeight_; };
		WindowInfo GetWindowInfo() const final;
		void* GetWindow() final { return window_; } // Returns the sdl window
		
		//Inherited from Subsystem interface

		int			Initialise() final;
		int			Shutdown() final;

		uint32_t flags_;
		SDL_Window* window_;
		int	screenWidth_;
		int	screenHeight_;

		bool vsync_ = true;
	};
}
