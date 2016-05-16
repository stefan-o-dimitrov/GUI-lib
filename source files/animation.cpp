#include "../include/GUI/Animation.h"

namespace gui
{
	Animation& Animation::setDuration(const float duration)
	{
		if (duration > 0) m_duration = duration;
		return *this;
	}

	Animation& Animation::setFPS(const float fps)
	{
		if (fps > 0) m_fps = fps;
		return *this;
	}

	const float Animation::getAnimationDuration() const
	{
		return m_duration;
	}

	const float Animation::getAnimationFPS() const
	{
		return m_fps;
	}

	void Animation::update() const
	{
		if (Duration(Internals::timeSinceStart() - m_timeOfLastStep).count() > 1.0f / m_fps)
		{
			step();
			m_timeOfLastStep = Internals::timeSinceStart();
		}
	}

	void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		update();
	}
}