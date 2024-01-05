#pragma once
#include "TimeSystem.h"
#include "AudioSystem.h"
#include "RenderSystem.h"
#include "EventSystem.h"
#include "WindowSystem.h"
#include "InputSystem.h"
#include "WorldSystem.h"
#include "ResourceSystem.h"
#include "UserInterfaceSystem.h"
#include "PhysicsSystem.h"
#include <ToffeeEngine.h>

using namespace Toffee::Internal;
namespace Toffee {
	
	// Service locator for accessing subsystems
	class ToffeeAPI	{
	public:
		// Access the time subsystem
		static TimeSystem* Time() { return &time_system_; };
		// Access the audio subsystem
		static AudioSystem* Audio() { return audio_system_.get(); };
		// Access the rendering subsystem
		static RenderSystem* Renderer() { return render_system_.get(); };
		// Access the event subsystem
		static EventSystem* Events() { return event_system_.get(); };
		// Access the window subsystem
		static WindowSystem* Window() { return window_system_.get(); };
		// Access the input subsystem
		static InputSystem* Input() { return input_system_.get(); };
		// Access the world subsystem
		static WorldSystem* World() { return world_system_.get(); };
		// Access the resource management subsystem
		static ResourceSystem* Resources() { return resource_system_.get(); };
		// Access the user interface subsystem
		static UserInterfaceSystem* UI() { return ui_system_.get(); };
		// Access the physics subsystem
		static PhysicsSystem* Physics() { return physics_system_.get(); };

	protected:
		friend class Core;
		friend class Subsystem;

		// Allows a susbsystem to self-register
		static void Provide(Subsystem* subsystem);

		// Initialise subsystems
		static void InitialiseSubsystems();

		// Shutdown subsytems
		static void ShutDownSubsystems();

		inline static TimeSystem time_system_;
		inline static std::unique_ptr<AudioSystem> audio_system_;
		inline static std::unique_ptr<RenderSystem> render_system_;
		inline static std::unique_ptr<EventSystem> event_system_;
		inline static std::unique_ptr<WindowSystem> window_system_;
		inline static std::unique_ptr<InputSystem> input_system_;
		inline static std::unique_ptr<WorldSystem> world_system_;
		inline static std::unique_ptr<ResourceSystem> resource_system_;
		inline static std::unique_ptr<UserInterfaceSystem> ui_system_;
		inline static std::unique_ptr<PhysicsSystem> physics_system_;
	};
}


