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

		virtual int GetWidth() const = 0;	
		virtual int GetHeight() const = 0;	
		virtual Engine::Internal::WindowInfo GetWindowInfo() const = 0;
		virtual void* GetWindow() = 0;

	protected:
		friend class EventSystem;
		void CreateRenderer();
		void Resize(float width, float height);
		virtual void HandleWindowEvent(WindowEvent& e) = 0;
	};
}
