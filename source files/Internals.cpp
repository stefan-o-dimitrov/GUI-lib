#include "../include/GUI/Internals.h"

namespace gui
{
	std::chrono::high_resolution_clock Internals::clock;
	unsigned char Internals::UPS = 15;

	const TimePoint Internals::timeSinceStart()
	{
		return clock.now();
	}

	void Internals::setUPS(const unsigned char updatesPerSecond)
	{
		UPS = updatesPerSecond;
	}

	const unsigned char Internals::getUPS()
	{
		return UPS;
	}
}