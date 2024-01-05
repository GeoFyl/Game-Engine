#pragma once
#include <chrono>
namespace Toffee::Internal {
	typedef std::chrono::high_resolution_clock Clock;

	// A simple time system
	class TimeSystem {
	public:

		// Returns the delta time, in seconds, since the last frame.
		double GetDeltaTime() { return delta_time_; }
		// Returns the current FPS based on the time since the last frame.
		double GetCurrentFPS() { return 1.f / delta_time_; }
		// Returns the average FPS, updates every 500ms.
		double GetAvgFPS() { return avg_fps_; }
	private:
		friend class Core;
		// Start the clock.
		void Start();
		
		// Called every frame.
		// Returns the delta time, in seconds, since the last frame.
		double Update();

		Clock::time_point previous_time_;
		double delta_time_;
		double avg_fps_ = 0.f;
		double fps_aggregate_ = 0.f;
		double fps_aggregate_time_ = 0.f;
		float update_counter_ = 0.f;
	};
}
