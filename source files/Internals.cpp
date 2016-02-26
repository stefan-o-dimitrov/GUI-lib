////////////////////////////////////////////////////////////
//
// GUI-lib - Graphical User Interface Library
// Copyright (C) 2014-2016 ShtGames (shtgamessts@gmail.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

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