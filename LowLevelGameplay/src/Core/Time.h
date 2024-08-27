#pragma once
#include <atomic>

namespace LLGP
{
	class Time
	{
	public:
		Time() = delete;

		//TODO: assess whether atomic is needed here
		static inline std::atomic<float> deltaTime;
		//TODO: replace this with a map of thread_id and delta for that thread, no need for atomic, keep main thread sync points
		static inline std::atomic<float> coroutineDeltaTime;
	};
}
