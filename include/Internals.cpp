#include "Internals.h"

namespace gui
{
	std::chrono::high_resolution_clock Internals::clock;

	const TimePoint Internals::timeSinceStart()
	{
		return clock.now();
	}
}