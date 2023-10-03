#include "pch.h"
#include "TimeSystem.h"

namespace Grengine {
	TimeSystem::TimeSystem(const int targetFramesPerSecond) : TickTime{ 0 }, TargetFrameTime{ 1000000 / 60 }
	{		
		LastUpdateTime = LastTickTime = Clock::now();
	}

	const std::chrono::nanoseconds TimeSystem::Update()
	{
		auto interval = Clock::now() - LastUpdateTime;
		LastUpdateTime = Clock::now();
		return interval;
	}
	const std::chrono::nanoseconds TimeSystem::Tick()
	{		
		auto interval = Clock::now() - LastTickTime;
		LastTickTime = Clock::now();
		TickTime += interval;
		return interval;
	}
	int TimeSystem::GetUpdateCount()
	{
		int updateCount = 0;
		while (TickTime > TargetFrameTime) {
			TickTime -= TargetFrameTime;
			updateCount++;
		}
		return updateCount;
	}
}