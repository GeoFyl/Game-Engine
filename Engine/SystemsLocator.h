#pragma once
#include "AudioSystem.h"
#include "RenderSystem.h"
#include "EventSystem.h"
#include "WindowSystem.h"

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
		static AudioSystem* Audio() { return audio_system_.get(); };
		static RenderSystem* Renderer() { return render_system_.get(); };
		static EventSystem* Events() { return event_system_.get(); };
		static WindowSystem* Window() { return window_system_.get(); };

	protected:
		friend class Core;
		friend class Subsystem;

		static void Provide(Subsystem* subsystem);
		static void InitialiseSubsystems();
		static void ShutDownSubsystems();

		inline static std::unique_ptr<AudioSystem> audio_system_;
		inline static std::unique_ptr<RenderSystem> render_system_;
		inline static std::unique_ptr<EventSystem> event_system_;
		inline static std::unique_ptr<WindowSystem> window_system_;
	};
}


