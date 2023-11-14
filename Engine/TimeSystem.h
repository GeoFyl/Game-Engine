#pragma once
#include <chrono>
namespace Engine::Internal {
	using Clock = std::chrono::steady_clock;
	/// <summary>
	/// Quick and dirty Time system.
	/// Manages time between "ticks" (main loops)
	/// and "updates" (update loops)
	/// 
	/// </summary>
	
	//TODO: Bug where the update loop is SLOWER than intended when the application isn't doing anything
	class TimeSystem {
	public:
		TimeSystem(const int targetFramesPerSecond);
		/// <summary>
		/// Call this when performing a game Update
		/// </summary>
		/// <returns>The time, in nanoseconds, since the last Update</returns>
		const std::chrono::nanoseconds	Update();

		/// <summary>
		/// Call this when processing a Tick
		/// </summary>
		/// <returns>The time, in nanoseconds, since the last Tick</returns>
		const std::chrono::nanoseconds	Tick();

		/// <summary>
		/// Used to find out whether we should be performing an Update or not
		/// </summary>
		/// <returns> How many Updates do we need to run.
		/// Usually 0, sometimes 1. Could be more than 1 if we've stalled.
		/// </returns>
		int GetUpdateCount();
	private:
		std::chrono::time_point<Clock>	LastUpdateTime;
		std::chrono::time_point<Clock>	LastTickTime;
		std::chrono::nanoseconds		TickTime;
		std::chrono::microseconds		TargetFrameTime;
	};
}
