#pragma once
namespace Grengine {
	/// <summary>
	/// Abstract interface for any Subsystem
	/// They need to be initialised and shut down
	/// </summary>
	class Subsystem {
	public:
		virtual int Initialise()	= 0;
		virtual int Shutdown()		= 0;
	};
}