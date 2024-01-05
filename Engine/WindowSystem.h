#pragma once
#include "Subsystem.h"
#include "WindowEvents.h"
#include <string>


namespace Toffee::Internal {
	//Handle to window and Device information
	struct WindowInfo {
		void* handle;
		void* device;
	};

	// Interface base class to window system.
	class WindowSystem : public Subsystem {
	public:

		WindowSystem() { type_ = SubsytemType::WINDOW; }
		// Open the window. Set name and dimensions.
		virtual int OpenWindow(std::string name, int width, int height) = 0;
		// Get the current width of the window.
		virtual int GetWidth() const = 0;	
		// Get the current height of the window.
		virtual int GetHeight() const = 0;	
		// Get data relating to the window 
		virtual Toffee::Internal::WindowInfo GetWindowInfo() const = 0;

		// Get some sort of pointer to the window.
		// Probably not required by all implementations.
		virtual void* GetWindow() = 0;

	protected:
		friend class EventSystem;
		friend class RenderSystem;
		// Receive and process an event.
		virtual void HandleWindowEvent(WindowEvent& e) = 0;

		// Make the rendering system create the renderer.
		void CreateRenderer();

		// Make the rendering system resize the viewport.
		void Resize(float width, float height);

	};
}
