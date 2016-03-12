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
		: Icon(visual), state(Idle)
	{
		Icon::spr.setColor(sf::Color(0.75 * 255, 0.75 * 255, 0.75 * 255, 255));
	}

	Button::Button(Icon&& visual)
		: Icon(std::move(visual))
	{
		Icon::spr.setColor(sf::Color(0.75 * 255, 0.75 * 255, 0.75 * 255, 255));
	}

	Button::Button(const Button& copy)
		: Icon(copy), onEvent(copy.onEvent), state(Idle)
	{
		Icon::spr.setColor(sf::Color(0.75 * 255, 0.75 * 255, 0.75 * 255, 255));

		if (copy.messageBuffer) messageBuffer.reset(new HoverMessage(*copy.messageBuffer));
		if (copy.name) name.reset(new TextArea(*copy.name));
		if (copy.predicates) predicates.reset(new PredicateArray(*copy.predicates));
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

		if (predicatesFulfilled && state != Idle)
			Icon::spr.setColor(sf::Color(0.85 * 255, 0.85 * 255, 0.85 * 255, 255));
		state = Idle;
	}

	const bool Button::input(const sf::Event& event)
	{
		switch (event.type)
		{
		case sf::Event::MouseMoved:
			if (Icon::input(event))
			{
				if (state == PressedDown) return true;
				if (predicatesFulfilled && state != Hot)
					Icon::spr.setColor(sf::Color(255, 255, 255, 255));
				state = Hot;
				return true;
			}
			else
			{
				if (predicatesFulfilled && state != Idle)
					Icon::spr.setColor(sf::Color(0.85 * 255, 0.85 * 255, 0.85 * 255, 255));
				state = Idle;
				return false;
			}
		case sf::Event::MouseButtonPressed:
			if (Icon::input(event))
			{
				if (onEvent.count(Pressed))
					onEvent.at(Pressed)();
				if (predicatesFulfilled && state != PressedDown)
					Icon::spr.setColor(sf::Color(0.7 * 255, 0.7 * 255, 0.7 * 255, 255));
				state = PressedDown;
				return true;
			}
			else return false;
		case sf::Event::MouseButtonReleased:
			if (Icon::input(event))
			{
				if (predicatesFulfilled)
				{
					if (state == PressedDown && onEvent.count(Released))
						onEvent.at(Released)();
					if (state != Hot)
						Icon::spr.setColor(sf::Color(255, 255, 255, 255));
					state = Hot;
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
		return state;
	}

	const std::shared_ptr<const HoverMessage> Button::getPredicateMessage()const
	{
		return predicatesFulfilled ? messageBuffer : message;
	}

	const std::shared_ptr<const HoverMessage> Button::getMessage()const
	{
		return predicatesFulfilled ? message : messageBuffer;
	}

	const std::shared_ptr<const TextArea> Button::getName()const
	{
		return name;
	}

	Button& Button::setName(const TextArea& newName)
	{
		name.reset(new TextArea(newName));
		name->message.reset();
		name->setPosition(getPosition().x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2,
			getPosition().y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
		return *this;
	}

	Button& Button::setName(TextArea&& tempName)
	{
		name.reset(new TextArea(std::move(tempName)));
		name->message.reset();
		name->updateFunction.reset();
		name->setPosition(getPosition().x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2,
			getPosition().y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
		return *this;
	}

	Button& Button::clearPredicateMessage()
	{
		if (predicatesFulfilled) messageBuffer.reset();
		else Hoverable::clearMessage();
		return *this;
	}

	Button& Button::clearMessage()
	{
		if (predicatesFulfilled) Hoverable::clearMessage();
		else messageBuffer.reset();
		return *this;
	}

	Button& Button::setMessage(const HoverMessage& newMessage)
	{
		predicatesFulfilled ? message.reset(new HoverMessage(newMessage)) :
			messageBuffer.reset(new HoverMessage(newMessage));
		return *this;
	}

	Button& Button::setMessage(HoverMessage&& tempMessage)
	{
		predicatesFulfilled ? message.reset(new HoverMessage(std::move(tempMessage))) :
			messageBuffer.reset(new HoverMessage(std::move(tempMessage)));
		return *this;
	}

	Button& Button::setPredicateMessage(const HoverMessage& newMessage)
	{
		predicatesFulfilled ? messageBuffer.reset(new HoverMessage(newMessage))
			: message.reset(new HoverMessage(newMessage));
		return *this;
	}

	Button& Button::setPredicateMessage(HoverMessage&& tempMessage)
	{
		predicatesFulfilled ? messageBuffer.reset(new HoverMessage(std::move(tempMessage)))
			: message.reset(new HoverMessage(std::move(tempMessage)));
		return *this;
	}

	Button& Button::clearPredicates()
	{
		predicates.reset();
		checkPredicates();
		return *this;
	}

	Button& Button::setPosition(const float x, const float y)
	{
		Icon::setPosition(x, y);
		if (name) name->setPosition(x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2,
			y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
		return *this;
	}

	Button& Button::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	Button& Button::setPredicates(const PredicateArray& newPredicates)
	{
		predicates.reset(new PredicateArray(newPredicates));
		return *this;
	}

	Button& Button::setPredicates(PredicateArray&& tempPredicates)
	{
		predicates.reset(new PredicateArray(std::move(tempPredicates)));
		return *this;
	}

	Button& Button::bindAction(const Event event, const std::function<void()>& action)
	{
		if (onEvent.count(event)) onEvent.at(event) = action;
		else onEvent.emplace(event, action);
		return *this;
	}

	Button& Button::bindAction(const Event event, std::function<void()>&& action)
	{
		if (onEvent.count(event)) onEvent.at(event) = std::move(action);
		else onEvent.emplace(event, std::move(action));
		return *this;
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		if (predicates && Duration(Internals::timeSinceStart() - timeOfLastPredicateCheck).count() > 1.0f / Internals::getUPS())
			checkPredicates();

		states.shader = !predicatesFulfilled && shaderLoadSuccessful ?
			&grayscaleShader : nullptr;

		target.draw(spr, states);
		if (name) target.draw(*name, states);
		Hoverable::draw(target, states);
	}

	void Button::checkPredicates()const
	{
		if (!predicates)
		{
			if (!predicatesFulfilled)
			{
				message.swap(messageBuffer);
				spr.setColor(sf::Color((1.0f - 0.15f * state) * 255, (1.0f - 0.15f * state) * 255, (1.0f - 0.15f * state) * 255, 255));
				predicatesFulfilled = true;
			}
			return;
		}

		for (auto it = predicates->begin(), end = predicates->end(); it != end; ++it)
			if (!(*it)())
			{
				if (predicatesFulfilled)
				{
					if (onEvent.count(PredicatesUnfulfilled)) onEvent.at(PredicatesUnfulfilled)();
					message.swap(messageBuffer);
					spr.setColor(sf::Color(255, 255, 255, 255));
					predicatesFulfilled = false;
				}
				return;
			}

		if (!predicatesFulfilled)
		{
			if (onEvent.count(PredicatesFulfilled)) onEvent.at(PredicatesFulfilled)();
			message.swap(messageBuffer);
			spr.setColor(sf::Color((1.0f - 0.15f * state) * 255, (1.0f - 0.15f * state) * 255, (1.0f - 0.15f * state) * 255, 255));
			predicatesFulfilled = true;
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