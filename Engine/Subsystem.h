#pragma once
namespace Toffee { class ToffeeAPI; }

namespace Toffee::Internal {

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
	/// </summary>
	class Subsystem {
	public:
	protected:
		virtual int Initialise()	= 0;
		virtual int Shutdown()		= 0;

		friend class Toffee::ToffeeAPI;
		// Used by subsystems to register themselves.
		void ProvideSystem(Subsystem* subsystem);
		SubsytemType type_;
	};
}