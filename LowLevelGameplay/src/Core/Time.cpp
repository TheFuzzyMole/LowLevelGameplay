#include <Core/Time.h>

namespace LLGP
{
	std::atomic<float> Time::deltaTime;
	std::atomic<float> Time::coroutineDeltaTime;
}
