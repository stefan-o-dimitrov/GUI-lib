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

#include "../include/GUI/CheckBox.h"

namespace gui
{
	CheckBox::CheckBox(const Button& fState, const Button& tState, const bool state)
		: Button(fState), trueState(tState), m_checked(state) 
	{
		setPosition(0, 0);
	}

	CheckBox::CheckBox(Button&& fState, Button&& tState, const bool state)
		: Button(std::move(fState)), trueState(std::move(tState)), m_checked(state)
	{
		setPosition(0, 0);
	}

	std::unique_ptr<Interactive> CheckBox::copy() const
	{ 
		return std::unique_ptr<CheckBox>(new CheckBox(*this));
	}

	std::unique_ptr<Interactive> CheckBox::move()
	{
		return std::unique_ptr<CheckBox>(new CheckBox(std::move(*this)));
	}

	void gui::CheckBox::lostFocus()
	{
		trueState.lostFocus();
		Button::lostFocus();
	}

	const bool CheckBox::input(const sf::Event& event)
	{
		if (m_checked)
		{
			if (event.type == sf::Event::MouseButtonReleased && trueState.getState() == PressedDown)
				m_checked = false;
			return trueState.input(event);
		}
		else
		{
			if (event.type == sf::Event::MouseButtonReleased && Button::getState() == PressedDown) 
				m_checked = true;
			return Button::input(event);
		}
	}

	const bool CheckBox::contains(const float x, const float y)const
	{
		if (m_checked) return trueState.contains(x, y);
		else return Button::contains(x, y);
	}

	const Button::State CheckBox::getState()const
	{
		if (m_checked) return trueState.getState();
		else return Button::getState();
	}

	const bool CheckBox::isChecked()const
	{
		return m_checked;
	}

	void CheckBox::toggle()
	{
		if (m_checked)
		{
			m_checked = false;
			if(trueState.m_onEvent.count(Released)) trueState.m_onEvent.at(Released)();
		}
		else
		{
			m_checked = true;
			if(Button::m_onEvent.count(Released)) Button::m_onEvent.at(Released)();
		}
	}
		
	CheckBox& CheckBox::setPosition(const float x, const float y)
	{
		trueState.setPosition(x + trueState.getPosition().x - Button::getPosition().x, y + trueState.getPosition().y - Button::getPosition().y);
		Button::setPosition(x, y);
		return *this;
	}

	CheckBox& CheckBox::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	CheckBox& CheckBox::setIsChecked(const bool state)
	{
		if (m_checked != state)	toggle();
		return *this;
	}

	void CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		if (m_checked) target.draw(trueState, states);
		else Button::draw(target, states);
	}
}