#pragma once

#include "Application.h"
#include "TimeSystem.h"
#include <memory>

namespace Engine::Internal {
	class Core
	{
	public:
		friend class Subsystem;

		Core(int argc, char** argv);

		int		Run();

	private:
		void	OpenWindow(std::string name, int ScreenWidth, int ScreenHeight);

		/// <summary>
		/// Get the event handler to process any events
		/// </summary>
		/// <returns>-1 on a Quit event</returns>
		int		ProcessEvents();
		int 	Update(double dt);
		void	Render();

		void	Close();

	private:
		std::unique_ptr<Engine::App::Application> app_;


		//Subsystems
		TimeSystem time_system_;
	};


}
