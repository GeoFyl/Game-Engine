#pragma once
#include <chrono>
namespace Engine::Internal {
	typedef std::chrono::high_resolution_clock Clock;

	/// <summary>
	/// A simple time system
	/// </summary>
	class TimeSystem {
	public:
		void Start();
		/// <summary>
		/// Call this when performing a game Update
		/// </summary>
		/// <returns>The delta time, in seconds, since the last frame</returns>
		double Update();
		/// <returns>The delta time, in seconds, since the last frame</returns>
		double GetDeltaTime() { return delta_time_; }
		/// <returns>The current FPS based on the time since the last frame</returns>
		double GetCurrentFPS() { return 1.f / delta_time_; }
		/// <returns>The average FPS, updates every 500ms</returns>
		double GetAvgFPS() { return avg_fps_; }
	private:
		Clock::time_point previous_time_;
		double delta_time_;
		double avg_fps_ = 0.f;
		double fps_aggregate_ = 0.f;
		double fps_aggregate_time_ = 0.f;
		float update_counter_ = 0.f;
	};
}
