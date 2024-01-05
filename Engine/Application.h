#pragma once

namespace Toffee::App {
	// Interface to the application layer
	// This should be supplied by the application project
	class Application
	{
	public:
		// Initialise application
		virtual void Start() = 0;
		// Application logic
		/// <param name="dt">Time since the last Update</param>
		virtual void Update(double dt) = 0;
		// Exit application
		void Exit() { exit_state_ = -1; }

		// For use by the engine
		const int GetExitState() const { return exit_state_; }

	protected:
		int exit_state_ = 0;
	};
	
	extern Application* CreateApp(int argc, char** argv);
}