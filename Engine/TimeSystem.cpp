#include "pch.h"
#include "TimeSystem.h"

using namespace Engine::Internal;

void Engine::Internal::TimeSystem::Start() {
	previous_time_ = Clock::now();
}

double TimeSystem::Update()
{
	Clock::time_point current_time = Clock::now();
	delta_time_ = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - previous_time_).count() / 1000000000.f;
	previous_time_ = current_time;

	fps_aggregate_time_ += delta_time_;
	fps_aggregate_ += 1.f / delta_time_;
	update_counter_++;
	if (fps_aggregate_time_ >= 0.5f) {
		avg_fps_ = fps_aggregate_ / update_counter_;
		fps_aggregate_time_ = 0.f;
		fps_aggregate_ = 0.f;
		update_counter_ = 0.f;
	}

	return delta_time_;
}
