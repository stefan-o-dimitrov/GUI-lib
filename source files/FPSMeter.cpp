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

#include "../include/GUI/FPSMeter.h"

namespace gui
{
	FPSMeter::FPSMeter(const sf::Font& font, const unsigned char characterSize)
	{
		setCharacterSize(characterSize);
		setFont(font);
	}
	std::unique_ptr<Interactive> FPSMeter::copy()const
	{
		return std::move(std::unique_ptr<gui::Interactive>(new FPSMeter(*this)));
	}

	std::unique_ptr<Interactive> FPSMeter::move()
	{
		return std::move(std::unique_ptr<gui::Interactive>(new FPSMeter(std::move(*this))));
	}

	const sf::FloatRect FPSMeter::getGlobalBounds() const
	{
		return m_text.getGlobalBounds();
	}

	const sf::Vector2f& FPSMeter::getPosition() const
	{
		return m_text.getPosition();
	}

	FPSMeter& FPSMeter::setPosition(const float x, const float y)
	{
		m_text.setPosition(x, y);
		return *this;
	}

	FPSMeter& FPSMeter::setPosition(const sf::Vector2f& position)
	{
		return setPosition(position.x, position.y);
	}

	const sf::Font& FPSMeter::getFont() const
	{
		return *m_text.getFont();
	}

	const unsigned char FPSMeter::getCharacterSize() const
	{
		return m_text.getCharacterSize();
	}

	const sf::Color& FPSMeter::getColor() const
	{
		return m_text.getColor();
	}

	FPSMeter& FPSMeter::setFont(const sf::Font& font)
	{
		m_text.setFont(font);
		return *this;
	}

	FPSMeter& FPSMeter::setCharacterSize(const unsigned char characterSize)
	{
		m_text.setCharacterSize(characterSize);
		return *this;
	}

	FPSMeter& FPSMeter::setColor(const sf::Color& color)
	{
		m_text.setColor(color);
		return *this;
	}

	void FPSMeter::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (Duration(Internals::timeSinceStart() - m_timeOfLastUpdate).count() >= 1.0f)
		{
			m_previousFrames = m_frames;
			m_text.setString(std::to_string(m_previousFrames));
			m_frames = 0;
			m_timeOfLastUpdate = Internals::timeSinceStart();
		}
		target.draw(m_text, states);
		m_frames++;
	}
}