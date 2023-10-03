#pragma once

#include "Grapplication.h"
#include "TimeSystem.h"
#include <memory>

namespace Grengine {
	class Core
	{
	public:
		Core(int argc, char** argv);

		int		Run();

	private:
		void	OpenWindow(int ScreenWidth, int ScreenHeight);

		/// <summary>
		/// Get the event handler to process any events
		/// </summary>
		/// <returns>-1 on a Quit event</returns>
		int		ProcessEvents();
		void	Update(double dt);
		void	Render();

		void	Close();

	private:
		std::unique_ptr<Grengine::Grapplication> grApp;
		TimeSystem grTimeSystem;
	};
}
