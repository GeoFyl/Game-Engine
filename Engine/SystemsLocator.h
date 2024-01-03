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

namespace Engine::Internal { class Core; }

using namespace Engine::Internal;
namespace Engine {
	//class Subsystem;
	//class AudioSystem;
	/// <summary>
	/// Service locator for accessing subsystems
	/// </summary>
	class SystemsAPI
	{
	public:
		static TimeSystem* Time() { return &time_system_; };
		static AudioSystem* Audio() { return audio_system_.get(); };
		static RenderSystem* Renderer() { return render_system_.get(); };
		static EventSystem* Events() { return event_system_.get(); };
		static WindowSystem* Window() { return window_system_.get(); };
		static InputSystem* Input() { return input_system_.get(); };
		static WorldSystem* World() { return world_system_.get(); };
		static ResourceSystem* Resources() { return resource_system_.get(); };
		static UserInterfaceSystem* UI() { return ui_system_.get(); };
		static PhysicsSystem* Physics() { return physics_system_.get(); };

	protected:
		friend class Core;
		friend class Subsystem;

		static void Provide(Subsystem* subsystem);
		static void InitialiseSubsystems();
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


