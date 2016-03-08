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
		: Button(fState), trueState(tState), checked(state)
	{
		if (name) name.reset();
		if (trueState.name) trueState.name.reset();
		setPosition(pos);
	}

	CheckBox::CheckBox(Button&& fState, Button&& tState, const sf::Vector2f& pos, const bool state)
		: Button(std::move(fState)), trueState(std::move(tState)), checked(state)
	{
		if (name) name.reset();
		if (trueState.name) trueState.name.reset();
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

	const bool CheckBox::input(const sf::Event& event)
	{
		if (checked)
		{
			if (event.type == sf::Event::MouseButtonReleased && trueState.getState() == PressedDown) checked = false;
			return trueState.input(event);
		}
		else
		{
			if (event.type == sf::Event::MouseButtonReleased && Button::getState() == PressedDown) checked = true;
			return Button::input(event);
		}
	}

	const bool CheckBox::contains(const sf::Vector2f& pos)const
	{
		if (checked) return trueState.contains(pos);
		else return Button::contains(pos);
	}

	const Button::State CheckBox::getState()const
	{
		if (checked) return trueState.getState();
		else return Button::getState();
	}

	const bool CheckBox::isChecked()const
	{
		return checked;
	}

	const sf::FloatRect CheckBox::getTrueStateGlobalBounds()const
	{
		return trueState.getGlobalBounds();
	}

	const sf::Texture& CheckBox::getTrueStateTexture()const
	{
		return trueState.getTexture();
	}

	const bool CheckBox::getTrueStateTransparencyCheck()const
	{
		return trueState.getTransparencyCheck();
	}

	const sf::IntRect& CheckBox::getTrueStateTextureRect()const
	{
		return trueState.getTextureRect();
	}

	const std::shared_ptr<const HoverMessage> CheckBox::getTrueStateMessage()const
	{
		return trueState.getMessage();
	}

	const std::shared_ptr<const HoverMessage> CheckBox::getTrueStatePredicateMessage()const
	{
		return trueState.getPredicateMessage();
	}

	void CheckBox::toggle()
	{
		if (checked)
		{
			checked = false;
			if(trueState.onEvent.count(Released)) trueState.onEvent.at(Released)();
		}
		else
		{
			checked = true;
			if(Button::onEvent.count(Released)) Button::onEvent.at(Released)();
		}
	}

	CheckBox& CheckBox::setIsChecked(const bool state)
	{
		if (checked != state)
			toggle();
		return *this;
	}

	CheckBox& CheckBox::setPosition(const float x, const float y)
	{
		Button::setPosition(x, y);
		trueState.setPosition(x + getGlobalBounds().width / 2.0f - trueState.getGlobalBounds().width / 2.0f, y + getGlobalBounds().height / 2.0f - trueState.getGlobalBounds().height / 2.0f);
		return *this;
	}

	CheckBox& CheckBox::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	CheckBox& CheckBox::setTrueStateTexture(const sf::Texture& tex, const bool transpCheck)
	{
		trueState.setTexture(tex, transpCheck);
		return *this;
	}

	CheckBox& CheckBox::setTrueStateTransparencyCheck(const bool transpCheck)
	{
		trueState.setTransparencyCheck(transpCheck);
		return *this;
	}

	CheckBox& CheckBox::setTrueStateTextureRect(const sf::IntRect& rect)
	{
		trueState.setTextureRect(rect);
		return *this;
	}

	CheckBox& CheckBox::setTrueStateMessage(const HoverMessage& newMessage)
	{
		trueState.setMessage(newMessage);
		return *this;
	}

	CheckBox& CheckBox::setTrueStateMessage(HoverMessage&& tempMessage)
	{
		trueState.setMessage(std::move(tempMessage));
		return *this;
	}

	CheckBox& CheckBox::setTrueStatePredicateMessage(const HoverMessage& newMessage)
	{
		trueState.setPredicateMessage(newMessage);
		return *this;
	}

	CheckBox& CheckBox::setTrueStatePredicateMessage(HoverMessage&& tempMessage)
	{
		trueState.setPredicateMessage(std::move(tempMessage));
		return *this;
	}

	CheckBox& CheckBox::setTrueStatePredicates(const PredicateArray& newPredicates)
	{
		trueState.setPredicates(newPredicates);
		return *this;
	}

	CheckBox& CheckBox::setTrueStatePredicates(PredicateArray&& predicates)
	{
		trueState.setPredicates(std::move(predicates));
		return *this;
	}

	void CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		if (checked) target.draw(trueState, states);
		else Button::draw(target, states);
	}
}