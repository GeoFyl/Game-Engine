#pragma once
#include "WindowSystem.h"

class SDL_Window;

namespace Engine::Internal {
	class WindowSystem_SDL : public WindowSystem {
	public:
		WindowSystem_SDL();
		~WindowSystem_SDL();



		//Inherited from RenderSystem interface
		int	OpenWindow(std::string name, int width, int height) final;
		void HandleWindowEvent(WindowEvent& e) final;
		int	GetWidth() const final { return screenWidth_; };
		int	GetHeight() const final { return screenHeight_; };
		WindowInfo GetWindowInfo() const final;
		void* GetWindow() { return window_; }
		void ToggleVsync();
	private:
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
