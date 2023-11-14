#pragma once
#include "Subsystem.h"
#include "WindowEvents.h"
#include <string>

namespace Engine::Internal {
	//Handle to window and Device information
	struct WindowInfo {
		void* handle;
		void* device;
	};

	class WindowSystem : public Subsystem {
	public:

		WindowSystem() { type_ = SubsytemType::WINDOW; }
		virtual int OpenWindow(std::string name, int width, int height) = 0;
		virtual void HandleWindowEvent(WindowEvent& e) = 0;
		virtual int GetScreenWidth() const = 0;	
		virtual int GetScreenHeight() const = 0;	
		virtual Engine::Internal::WindowInfo GetWindowInfo() const = 0;
	};
}
