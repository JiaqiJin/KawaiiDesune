#pragma once

namespace Physic
{
	class GameTime
	{
	public:
		static float deltaTime;	// The delta time in seconds

		static inline double FindTimeDiffSeconds(const std::chrono::high_resolution_clock::time_point& t1, const std::chrono::high_resolution_clock::time_point& t2)
		{
			return std::chrono::duration<double>(t1 - t2).count();
		}

		static inline void Init() { currentTime = GetTime(); }

		static inline void Update()
		{
			prevCurrentTime = currentTime;
			currentTime = GetTime();
			CalculateDeltaTime();
		}

		static inline float GetTimeSeconds() { return (float)FindTimeDiffSeconds(GetTime(), startTime); }

		static inline std::chrono::high_resolution_clock::time_point GetTime() { return std::chrono::high_resolution_clock::now(); }

	private:
		static const std::chrono::high_resolution_clock::time_point startTime;
		static std::chrono::high_resolution_clock::time_point currentTime;
		static std::chrono::high_resolution_clock::time_point prevCurrentTime;

		static inline void CalculateDeltaTime()
		{
			deltaTime = (float)FindTimeDiffSeconds(currentTime, prevCurrentTime);
		}
	};
}