#pragma once

namespace Engine::App {
	/// <summary>
	/// Interface to the application layer
	/// This should be supplied by the Game project
	/// </summary>
	class Application
	{
	public:
		
		/// <summary>
		/// Game logic
		/// </summary>
		/// <param name="dt">Time since the last Update</param>
		virtual void Start() = 0;
		virtual void Update(double dt) = 0;
		void Exit() { exit_state_ = -1; }
		const int GetExitState() const { return exit_state_; }

	protected:
		int exit_state_ = 0;
	};
	
	extern Application* CreateApp(int argc, char** argv);
}