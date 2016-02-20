#pragma once

#include <chrono>

namespace gui
{
	typedef std::chrono::time_point<std::chrono::high_resolution_clock>(TimePoint);
	typedef std::chrono::duration<float>(Duration);

	class Internals 
	{
	public:
		static const TimePoint timeSinceStart();
	private:
		static std::chrono::high_resolution_clock clock;
	};
};