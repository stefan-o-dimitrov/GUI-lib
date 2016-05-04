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

#include "../include/GUI/Button.h"
#include "../include/GUI/Window.h"

#include <sstream>

namespace gui
{
	const std::string GRAYSCALE_SHADER_CODE =
		"uniform sampler2D tex;\
		\
		void main()\
		{\
			vec4 color = texture2D( tex, gl_TexCoord[0].xy ) * gl_Color;\
			float greyValue = color.r * 0.29 + color.g * 0.58 + color.b * 0.13;\
			gl_FragColor = vec4(greyValue, greyValue, greyValue, color.a);\
		}";
	sf::Shader Button::grayscaleShader;
	const bool Button::shaderLoadSuccessful = Button::loadShader();

	Button::Button(const Icon& visual)
		: Icon(visual), m_state(Idle)
	{
		Icon::m_icon.setColor(sf::Color(0.75 * 255, 0.75 * 255, 0.75 * 255, 255));
	}

	Button::Button(Icon&& visual)
		: Icon(std::move(visual))
	{
		Icon::m_icon.setColor(sf::Color(0.75 * 255, 0.75 * 255, 0.75 * 255, 255));
	}

	Button::Button(const Button& copy)
		: Icon(copy), m_onEvent(copy.m_onEvent), m_state(Idle)
	{
		Icon::m_icon.setColor(sf::Color(0.75 * 255, 0.75 * 255, 0.75 * 255, 255));

		if (copy.m_messageBuffer) m_messageBuffer.reset(new HoverMessage(*copy.m_messageBuffer));
		if (copy.m_name) m_name.reset(new TextArea(*copy.m_name));
		if (copy.m_predicates) m_predicates.reset(new PredicateArray(*copy.m_predicates));
	}

	std::unique_ptr<Interactive> Button::copy() const
	{ 
		return std::unique_ptr<Button>(new Button(*this));
	}

	std::unique_ptr<Interactive> Button::move()
	{ 
		return std::unique_ptr<Button>(new Button(std::move(*this)));
	}

	void gui::Button::lostFocus()
	{
		Icon::lostFocus();

		if (m_predicatesFulfilled && m_state != Idle)
			Icon::m_icon.setColor(sf::Color(0.85 * 255, 0.85 * 255, 0.85 * 255, 255));
		m_state = Idle;
	}

	const bool Button::input(const sf::Event& event)
	{
		switch (event.type)
		{
		case sf::Event::MouseMoved:
			if (Icon::input(event))
			{
				if (m_state == PressedDown) return true;
				if (m_predicatesFulfilled && m_state != Hot)
					Icon::m_icon.setColor(sf::Color(255, 255, 255, 255));
				m_state = Hot;
				return true;
			}
			else
			{
				if (m_predicatesFulfilled && m_state != Idle)
					Icon::m_icon.setColor(sf::Color(0.85 * 255, 0.85 * 255, 0.85 * 255, 255));
				m_state = Idle;
				return false;
			}
		case sf::Event::MouseButtonPressed:
			if (Icon::input(event))
			{
				if (m_onEvent.count(Pressed))
					m_onEvent.at(Pressed)();
				if (m_predicatesFulfilled && m_state != PressedDown)
					Icon::m_icon.setColor(sf::Color(0.7 * 255, 0.7 * 255, 0.7 * 255, 255));
				m_state = PressedDown;
				return true;
			}
			else return false;
		case sf::Event::MouseButtonReleased:
			if (Icon::input(event))
			{
				if (m_predicatesFulfilled)
				{
					if (m_state == PressedDown && m_onEvent.count(Released))
						m_onEvent.at(Released)();
					if (m_state != Hot)
						Icon::m_icon.setColor(sf::Color(255, 255, 255, 255));
					m_state = Hot;
				}
				return true;
			}
			else return false;
		default:
			return false;
		}
	}

	const Button::State Button::getState()const
	{
		return m_state;
	}

	const std::shared_ptr<const HoverMessage> Button::getPredicateMessage()const
	{
		return m_predicatesFulfilled ? m_messageBuffer : m_message;
	}

	const std::shared_ptr<HoverMessage> Button::getMessage()const
	{
		return m_predicatesFulfilled ? m_message : m_messageBuffer;
	}

	const std::shared_ptr<const TextArea> Button::getName()const
	{
		return m_name;
	}

	Button& Button::setName(const TextArea& newName)
	{
		m_name.reset(new TextArea(newName));
		m_name->m_message.reset();
		m_name->setPosition(getPosition().x + getGlobalBounds().width / 2 - m_name->getGlobalBounds().width / 2,
			getPosition().y + getGlobalBounds().height / 2 - m_name->getGlobalBounds().height / 2);
		return *this;
	}

	Button& Button::setName(TextArea&& tempName)
	{
		m_name.reset(new TextArea(std::move(tempName)));
		m_name->m_message.reset();
		m_name->m_updateFunction.reset();
		m_name->setPosition(getPosition().x + getGlobalBounds().width / 2 - m_name->getGlobalBounds().width / 2,
			getPosition().y + getGlobalBounds().height / 2 - m_name->getGlobalBounds().height / 2);
		return *this;
	}

	Button& Button::clearPredicateMessage()
	{
		if (m_predicatesFulfilled) m_messageBuffer.reset();
		else Hoverable::clearMessage();
		return *this;
	}

	Button& Button::clearMessage()
	{
		if (m_predicatesFulfilled) Hoverable::clearMessage();
		else m_messageBuffer.reset();
		return *this;
	}

	Button& Button::setMessage(const HoverMessage& newMessage)
	{
		m_predicatesFulfilled ? m_message.reset(new HoverMessage(newMessage)) :
			m_messageBuffer.reset(new HoverMessage(newMessage));
		return *this;
	}

	Button& Button::setMessage(HoverMessage&& tempMessage)
	{
		m_predicatesFulfilled ? m_message.reset(new HoverMessage(std::move(tempMessage))) :
			m_messageBuffer.reset(new HoverMessage(std::move(tempMessage)));
		return *this;
	}

	Button& Button::setTexture(const sf::Texture& texture, const bool transparencyCheck)
	{
		Icon::setTexture(texture, transparencyCheck);
		if (m_name)
			m_name->setPosition(getPosition().x + getGlobalBounds().width / 2 - m_name->getGlobalBounds().width / 2,
				getPosition().y + getGlobalBounds().height / 2 - m_name->getGlobalBounds().height / 2);
		return *this;
	}

	Button& Button::setColor(const sf::Color& color)
	{
		Icon::setColor(color);
		return *this;
	}

	Button& Button::setPredicateMessage(const HoverMessage& newMessage)
	{
		m_predicatesFulfilled ? m_messageBuffer.reset(new HoverMessage(newMessage))
			: m_message.reset(new HoverMessage(newMessage));
		return *this;
	}

	Button& Button::setPredicateMessage(HoverMessage&& tempMessage)
	{
		m_predicatesFulfilled ? m_messageBuffer.reset(new HoverMessage(std::move(tempMessage)))
			: m_message.reset(new HoverMessage(std::move(tempMessage)));
		return *this;
	}

	Button& Button::clearPredicates()
	{
		m_predicates.reset();
		checkPredicates();
		return *this;
	}

	Button& Button::setPosition(const float x, const float y)
	{
		Icon::setPosition(x, y);
		if (m_name) m_name->setPosition(x + getGlobalBounds().width / 2 - m_name->getGlobalBounds().width / 2,
			y + getGlobalBounds().height / 2 - m_name->getGlobalBounds().height / 2);
		return *this;
	}

	Button& Button::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	Button& Button::setPredicates(const PredicateArray& newPredicates)
	{
		m_predicates.reset(new PredicateArray(newPredicates));
		return *this;
	}

	Button& Button::setPredicates(PredicateArray&& tempPredicates)
	{
		m_predicates.reset(new PredicateArray(std::move(tempPredicates)));
		return *this;
	}

	Button& Button::bindAction(const Event event, const std::function<void()>& action)
	{
		if (m_onEvent.count(event)) m_onEvent.at(event) = action;
		else m_onEvent.emplace(event, action);
		return *this;
	}

	Button& Button::bindAction(const Event event, std::function<void()>&& action)
	{
		if (m_onEvent.count(event)) m_onEvent.at(event) = std::move(action);
		else m_onEvent.emplace(event, std::move(action));
		return *this;
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		if (m_predicates && Duration(Internals::timeSinceStart() - m_timeOfLastPredicateCheck).count() > 1.0f / Internals::getUPS())
			checkPredicates();

		states.shader = !m_predicatesFulfilled && shaderLoadSuccessful ?
			&grayscaleShader : nullptr;

		target.draw(m_icon, states);
		if (m_name) target.draw(*m_name, states);
		Hoverable::draw(target, states);
	}

	void Button::checkPredicates()const
	{
		if (!m_predicates)
		{
			if (!m_predicatesFulfilled)
			{
				m_message.swap(m_messageBuffer);
				m_icon.setColor(sf::Color((1.0f - 0.15f * m_state) * 255, (1.0f - 0.15f * m_state) * 255, (1.0f - 0.15f * m_state) * 255, 255));
				m_predicatesFulfilled = true;
			}
			return;
		}

		for (auto it = m_predicates->begin(), end = m_predicates->end(); it != end; ++it)
			if (!(*it)())
			{
				if (m_predicatesFulfilled)
				{
					if (m_onEvent.count(PredicatesUnfulfilled)) m_onEvent.at(PredicatesUnfulfilled)();
					m_message.swap(m_messageBuffer);
					m_icon.setColor(sf::Color(255, 255, 255, 255));
					m_predicatesFulfilled = false;
				}
				return;
			}

		if (!m_predicatesFulfilled)
		{
			if (m_onEvent.count(PredicatesFulfilled)) m_onEvent.at(PredicatesFulfilled)();
			m_message.swap(m_messageBuffer);
			m_icon.setColor(sf::Color((1.0f - 0.15f * m_state) * 255, (1.0f - 0.15f * m_state) * 255, (1.0f - 0.15f * m_state) * 255, 255));
			m_predicatesFulfilled = true;
		}
	}

	const bool Button::loadShader()
	{
		if (grayscaleShader.loadFromMemory(GRAYSCALE_SHADER_CODE, sf::Shader::Fragment))
		{
			grayscaleShader.setParameter("tex", sf::Shader::CurrentTexture);
			return true;
		}
		else return false;
	}
}