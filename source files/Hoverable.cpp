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

#include "../include/GUI/WindowManager.h"
#include "../include/GUI/Window.h"
#include "../include/GUI/Hoverable.h"
#include "../include/GUI/Internals.h"

namespace gui
{
	void gui::Hoverable::lostFocus()
	{
		mouseLeft();
	}

	const bool Hoverable::input(const sf::Event& event)
	{
		switch (event.type)
		{
		case sf::Event::MouseMoved:
		{
			const sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
			if (!contains(mousePos)) { mouseLeft(); return false; }
			else { if (!m_hasBeenPressed) mouseEntered(mousePos); return true; }
		}
		case sf::Event::MouseButtonPressed:
			mouseLeft();
			m_hasBeenPressed = true;
			return contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
		case sf::Event::MouseButtonReleased:
			return contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
		default:
			return false;
		}
	}


	const std::shared_ptr<HoverMessage> Hoverable::getMessage() const
	{
		return m_message;
	}


	Hoverable& Hoverable::clearMessage()
	{
		m_message.reset();
		return *this;
	}


	Hoverable& Hoverable::setMessage(const HoverMessage& newMessage)
	{
		m_message.reset(new HoverMessage(newMessage));
		return *this;
	}


	Hoverable& Hoverable::setMessage(HoverMessage&& tempMessage)
	{
		m_message.reset(new HoverMessage(std::move(tempMessage)));
		return *this;
	}


	Hoverable& Hoverable::setDelay(const float newDelay)
	{
		newDelay >= 0.0f ? m_delay = newDelay : m_delay = 0.0f;
		return *this;
	}

	Hoverable::Hoverable(const HoverMessage& newMessage, const float newDelay)
		: m_message(new HoverMessage(newMessage))
	{
		setDelay(newDelay);
	}

	Hoverable::Hoverable(const Hoverable& copy)
		: Interactive(copy)
	{
		setDelay(copy.m_delay);
		if (copy.m_message) m_message.reset(new HoverMessage(*copy.m_message));
	}

	Hoverable& Hoverable::operator=(const Hoverable& copy)
	{
		Interactive::operator=(copy);
		setDelay(copy.m_delay);
		if (copy.m_message) m_message.reset(new HoverMessage(*copy.m_message));
		return *this;
	}

	void Hoverable::mouseEntered(const sf::Vector2f& mousePos)
	{
		if (!m_timeMouseEntered) 
			m_timeMouseEntered.reset(new TimePoint(Internals::timeSinceStart()));
		if (m_message && m_message->getFadeAmount() != 1.0f)
			m_message->setPosition(mousePos.x, mousePos.y + 32);
	}

	void Hoverable::mouseLeft()
	{
		m_hasBeenPressed = false;
		m_timeMouseEntered.reset();
	}

	const bool Hoverable::hasMessageDelayPassed() const
	{
		return m_timeMouseEntered ? Duration(Internals::timeSinceStart() - *m_timeMouseEntered).count() > m_delay : false;
	}

	void Hoverable::display(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_message)
		{
			m_message->setFadeDirection(hasMessageDelayPassed());
			target.draw(*m_message, states);
		}
	}

	void Hoverable::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_message)
		{
			if (m_parent)
			{ 
				if (!Window::m_message || Window::m_message->getMessage()->getFadeAmount() == 0.0f)
					Window::m_message = this;
			}
			else display(target, states);
		}
	}
}
