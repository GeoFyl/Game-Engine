#pragma once
namespace Engine { class SystemsAPI; }

namespace Engine::Internal {
	enum class SubsytemType {
		RENDER,
		AUDIO,
		EVENT,
		WINDOW,
		INPUT,
		UI,
		WORLD,
		RESOURCE,
		PHYSICS
	};

	/// <summary>
	/// Abstract interface for any Subsystem
	/// They need to be initialised and shut down
	/// </summary>
	class Subsystem {
	public:
	protected:
		virtual int Initialise()	= 0;
		virtual int Shutdown()		= 0;

		friend class Engine::SystemsAPI;
		void ProvideSystem(Subsystem* subsystem);
		SubsytemType type_;
	};
}