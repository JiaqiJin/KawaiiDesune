#include "../PrecompiledHeader.h"
#include "GameTime.h"

namespace Physic
{
	const std::chrono::high_resolution_clock::time_point GameTime::startTime = GetTime();
	std::chrono::high_resolution_clock::time_point GameTime::currentTime = startTime;
	std::chrono::high_resolution_clock::time_point GameTime::prevCurrentTime;
	float GameTime::deltaTime = 0.f;
}