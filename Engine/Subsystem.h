#pragma once
namespace Engine { class SystemsAPI; }

namespace Engine::Internal {
	enum class SubsytemType {
		RENDER,
		AUDIO,
		EVENT,
		WINDOW
	};

	/// <summary>
	/// Abstract interface for any Subsystem
	/// They need to be initialised and shut down
	/// </summary>
	class Subsystem {
	public:
		virtual int Initialise()	= 0;
		virtual int Shutdown()		= 0;
	protected:
		friend class Engine::SystemsAPI;
		void ProvideSystem(Subsystem* subsystem);
		SubsytemType type_;
	};
}