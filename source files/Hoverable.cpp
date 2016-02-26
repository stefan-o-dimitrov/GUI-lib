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

#include "../include/GUI/Hoverable.h"
#include "../include/GUI/Internals.h"

namespace gui
{

	const bool Hoverable::input(const sf::Event& event)
	{
		switch (event.type)
		{
		case sf::Event::MouseMoved:
		{
			const sf::Vector2f mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
			if (!contains(mousePos)) { mouseLeft(); return false; }
			else { if (!hasBeenPressed) mouseEntered(mousePos); return true; }
		}
		case sf::Event::MouseButtonPressed:
			mouseLeft();
			hasBeenPressed = true;
			return contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
		case sf::Event::MouseButtonReleased:
			return contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
		default:
			return false;
		}
	}


	const std::shared_ptr<const HoverMessage> Hoverable::getMessage() const
	{
		return message;
	}


	Hoverable& Hoverable::clearMessage()
	{
		message.reset();
		return *this;
	}


	Hoverable& Hoverable::setMessage(const HoverMessage& newMessage)
	{
		message.reset(new HoverMessage(newMessage));
		return *this;
	}


	Hoverable& Hoverable::setMessage(HoverMessage&& tempMessage)
	{
		message.reset(new HoverMessage(std::move(tempMessage)));
		return *this;
	}


	Hoverable& Hoverable::setDelay(const float newDelay)
	{
		newDelay >= 0.0f ? delay = newDelay : delay = 0.0f;
		return *this;
	}


	Hoverable::Hoverable(const HoverMessage& newMessage, const float newDelay)
		: message(new HoverMessage(newMessage))
	{
		setDelay(newDelay);
	}

	Hoverable::Hoverable(const Hoverable& copy)
	{
		setDelay(copy.delay);
		if (copy.message) message.reset(new HoverMessage(*copy.message));
	}

	Hoverable& Hoverable::operator=(const Hoverable& copy)
	{
		setDelay(copy.delay);
		if (copy.message) message.reset(new HoverMessage(*copy.message));
		return *this;
	}

	void Hoverable::mouseEntered(const sf::Vector2f& mousePos)
	{
		if (!timeMouseEntered) timeMouseEntered.reset(new TimePoint(Internals::timeSinceStart()));
		if (message && message->getFadeAmount() != 1.0f) message->setPosition(mousePos.x, mousePos.y + 32);
	}

	void Hoverable::mouseLeft()
	{
		hasBeenPressed = false;
		timeMouseEntered.reset();
	}

	const bool Hoverable::hasMessageDelayPassed() const
	{
		return timeMouseEntered ? Duration(Internals::timeSinceStart() - *timeMouseEntered).count() > delay :
			false;
	}

	void Hoverable::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (message)
		{
			message->setFadeDirection(hasMessageDelayPassed());
			target.draw(*message, states);
		}
	}
}
