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

#ifndef GUI_SLIDER
#define GUI_SLIDER

#include <functional>

#include <SFML/Graphics/RectangleShape.hpp>

#include "Button.h"
#include "Hoverable.h"

namespace gui 
{
	class Slider final : public Hoverable
	{
	public:
		const bool input(const sf::Event& event)override;
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
		void setSliderPosition(float x);

		std::function<void(const float)> m_valueSetter;
		sf::Vector2f                     m_range;
		sf::RectangleShape               m_sliderBar;
		Button                           m_slider;
		std::unique_ptr<float>           m_dragOffset;
		std::unique_ptr<Button>          m_plus, m_minus;
	};
};

#endif