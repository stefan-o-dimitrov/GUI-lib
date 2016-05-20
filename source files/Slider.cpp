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

#include "../include/GUI/Slider.h"

namespace gui
{
	const bool Slider::input(const sf::Event& event)
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			if (m_slider.input(event) || (m_plus && m_plus->input(event)) || (m_minus && m_minus->input(event)))
				return true;
			else if (m_sliderBar.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
			{
				setSliderPosition(event.mouseButton.x - m_slider.getGlobalBounds().width / 2);
				return true;
			}
			return false;
		case sf::Event::MouseButtonReleased:
			break;
		case sf::Event::MouseMoved:
			break;
		}
		return false;
	}

	void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{

	}

	void Slider::setSliderPosition(float x)
	{
		if (x < m_sliderBar.getPosition().x) x = m_sliderBar.getPosition().x;
		else if (x > m_sliderBar.getPosition().x + m_sliderBar.getGlobalBounds().width)
			x = m_sliderBar.getPosition().x + m_sliderBar.getGlobalBounds().width - m_slider.getGlobalBounds().width;

		if (m_minus && x < m_minus->getPosition().x + m_minus->getGlobalBounds().width)
				x = m_minus->getPosition().x + m_minus->getGlobalBounds().width;
		if (m_plus && x + m_slider.getGlobalBounds().width > m_plus->getPosition().x)
			x = m_plus->getPosition().x - m_slider.getGlobalBounds().width;

		m_slider.setPosition(x, m_slider.getPosition().y);

	}
}
