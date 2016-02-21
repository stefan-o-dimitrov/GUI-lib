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
		static void setUPS(const unsigned char updatesPerSecond);
		static const unsigned char getUPS();

	private:
		static std::chrono::high_resolution_clock clock;
		static unsigned char UPS;
	};
};