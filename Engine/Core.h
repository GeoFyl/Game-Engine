#pragma once
#include "Application.h"
#include "TimeSystem.h"
#include <memory>

namespace Toffee::Internal {
	class Core
	{
	public:
		friend class Subsystem;

		Core(int argc, char** argv);

		int		Run();

	private:

		// Process any events
		int		ProcessEvents();

		// Update anything which needs updated
		int 	Update(double dt);

		// Render
		void	Render();

		// Before exiting the program
		void	Close();

	private:
		std::unique_ptr<Toffee::App::Application> app_;
	};


}
