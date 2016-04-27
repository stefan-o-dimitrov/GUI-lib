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
	CheckBox::CheckBox(const Button& fState, const Button& tState, const sf::Vector2f& pos, const bool state)
		: Button(fState), m_trueState(tState), m_checked(state)
	{
		if (m_name) m_name.reset();
		if (m_trueState.m_name) m_trueState.m_name.reset();
		setPosition(pos);
	}

	CheckBox::CheckBox(Button&& fState, Button&& tState, const sf::Vector2f& pos, const bool state)
		: Button(std::move(fState)), m_trueState(std::move(tState)), m_checked(state)
	{
		if (m_name) m_name.reset();
		if (m_trueState.m_name) m_trueState.m_name.reset();
		setPosition(pos);
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
		m_checked ? m_trueState.lostFocus() : Button::lostFocus();
	}

	const bool CheckBox::input(const sf::Event& event)
	{
		if (m_checked)
		{
			if (event.type == sf::Event::MouseButtonReleased && m_trueState.getState() == PressedDown) 
				m_checked = false;
			return m_trueState.input(event);
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
		if (m_checked) return m_trueState.contains(x, y);
		else return Button::contains(x, y);
	}

	const Button::State CheckBox::getState()const
	{
		if (m_checked) return m_trueState.getState();
		else return Button::getState();
	}

	const bool CheckBox::isChecked()const
	{
		return m_checked;
	}

	const sf::FloatRect CheckBox::getTrueStateGlobalBounds()const
	{
		return m_trueState.getGlobalBounds();
	}

	const sf::Texture& CheckBox::getTrueStateTexture()const
	{
		return m_trueState.getTexture();
	}

	const bool CheckBox::getTrueStateTransparencyCheck()const
	{
		return m_trueState.getTransparencyCheck();
	}

	const sf::IntRect& CheckBox::getTrueStateTextureRect()const
	{
		return m_trueState.getTextureRect();
	}

	const std::shared_ptr<const HoverMessage> CheckBox::getTrueStateMessage()const
	{
		return m_trueState.getMessage();
	}

	const std::shared_ptr<const HoverMessage> CheckBox::getTrueStatePredicateMessage()const
	{
		return m_trueState.getPredicateMessage();
	}

	void CheckBox::toggle()
	{
		if (m_checked)
		{
			m_checked = false;
			if(m_trueState.m_onEvent.count(Released)) m_trueState.m_onEvent.at(Released)();
		}
		else
		{
			m_checked = true;
			if(Button::m_onEvent.count(Released)) Button::m_onEvent.at(Released)();
		}
	}

	CheckBox& CheckBox::setIsChecked(const bool state)
	{
		if (m_checked != state)
			toggle();
		return *this;
	}

	CheckBox& CheckBox::setPosition(const float x, const float y)
	{
		Button::setPosition(x, y);
		m_trueState.setPosition(x + getGlobalBounds().width / 2.0f - m_trueState.getGlobalBounds().width / 2.0f, y + getGlobalBounds().height / 2.0f - m_trueState.getGlobalBounds().height / 2.0f);
		return *this;
	}

	CheckBox& CheckBox::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	CheckBox& CheckBox::setTrueStateTexture(const sf::Texture& tex, const bool transpCheck)
	{
		m_trueState.setTexture(tex, transpCheck);
		return *this;
	}

	CheckBox& CheckBox::setTrueStateTransparencyCheck(const bool transpCheck)
	{
		m_trueState.setTransparencyCheck(transpCheck);
		return *this;
	}

	CheckBox& CheckBox::setTrueStateTextureRect(const sf::IntRect& rect)
	{
		m_trueState.setTextureRect(rect);
		return *this;
	}

	CheckBox& CheckBox::setTrueStateMessage(const HoverMessage& newMessage)
	{
		m_trueState.setMessage(newMessage);
		return *this;
	}

	CheckBox& CheckBox::setTrueStateMessage(HoverMessage&& tempMessage)
	{
		m_trueState.setMessage(std::move(tempMessage));
		return *this;
	}

	CheckBox& CheckBox::setTrueStatePredicateMessage(const HoverMessage& newMessage)
	{
		m_trueState.setPredicateMessage(newMessage);
		return *this;
	}

	CheckBox& CheckBox::setTrueStatePredicateMessage(HoverMessage&& tempMessage)
	{
		m_trueState.setPredicateMessage(std::move(tempMessage));
		return *this;
	}

	CheckBox& CheckBox::setTrueStatePredicates(const PredicateArray& newPredicates)
	{
		m_trueState.setPredicates(newPredicates);
		return *this;
	}

	CheckBox& CheckBox::setTrueStatePredicates(PredicateArray&& predicates)
	{
		m_trueState.setPredicates(std::move(predicates));
		return *this;
	}

	void CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		if (m_checked) target.draw(m_trueState, states);
		else Button::draw(target, states);
	}
}