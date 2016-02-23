#include "../include/GUI/Button.h"
#include "../include/GUI/AudioSystem.h"

#include <sstream>

namespace gui
{
	const std::string Button::STATE_SHADER =
		"uniform sampler2D tex;\
		\
		void main()\
		{\
			vec4 color = texture2D( tex, gl_TexCoord[0].xy ) * gl_Color;\
			float greyValue = color.r * 0.29 + color.g * 0.58 + color.b * 0.13;\
			gl_FragColor = vec4(greyValue, greyValue, greyValue, color.a);\
		}";
	sf::Shader Button::shader;
	const bool Button::shaderLoadSuccessful = Button::loadShader();

	Button::Button(const Icon& visual, const std::function<void()>& onClick)
		: Icon(visual), onClickAction(onClick), state(Idle) 
	{
		Icon::spr.setColor(sf::Color(0.75 * 255, 0.75 * 255, 0.75 * 255, 255));
	}

	Button::Button(Icon&& visual, std::function<void()>&& onClick)
		: Icon(std::move(visual)), onClickAction(std::move(onClick)) 
	{
		Icon::spr.setColor(sf::Color(0.75 * 255, 0.75 * 255, 0.75 * 255, 255));
	}

	Button::Button(const Button& copy)
		: Icon(copy), onClickAction(copy.onClickAction), state(Idle)
	{
		Icon::spr.setColor(sf::Color(0.75 * 255, 0.75 * 255, 0.75 * 255, 255));

		if (copy.messageBuffer) messageBuffer.reset(new HoverMessage(*copy.messageBuffer));
		if (copy.stringBuffer) stringBuffer.reset(new ColoredText(*copy.stringBuffer));
		if (copy.name) name.reset(new TextArea(*copy.name));
		if (copy.sound) sound.reset(new unsigned short(*copy.sound));
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
					if (state == PressedDown)
					{
						if (sound) AudioSystem::playSound(*sound);
						onClickAction();
					}
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

	const State Button::getState()const
	{
		return state;
	}

	const std::shared_ptr<const HoverMessage> Button::getPredicateMessage()const
	{
		if (predicatesFulfilled) return messageBuffer;
		else return message;
	}

	const std::shared_ptr<const HoverMessage> Button::getMessage()const
	{
		if (predicatesFulfilled) return message;
		else return messageBuffer;
	}

	const std::shared_ptr<const TextArea> Button::getName()const
	{
		return name;
	}

	const std::shared_ptr<const unsigned short> Button::getClickSound()const
	{
		return sound;
	}

	Button& Button::setName(const TextArea& newName)
	{
		name.reset(new TextArea(newName));
		name->message.reset();
		name->setPosition(getPosition().x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, getPosition().y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
		return *this;
	}

	Button& Button::setName(TextArea&& tempName)
	{
		name.reset(new TextArea(std::move(tempName)));
		name->message.reset();
		name->updateFunction.reset();
		name->setPosition(getPosition().x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, getPosition().y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
		return *this;
	}

	Button& Button::setClickSound(const unsigned short newSound)
	{
		sound ? *sound = newSound : sound.reset(new unsigned short(newSound));
		return *this;
	}

	Button& Button::clearMessage()
	{
		Hoverable::clearMessage();
		return *this;
	}

	Button& Button::setMessage(const HoverMessage& newMessage)
	{
		if (predicatesFulfilled) message.reset(new HoverMessage(newMessage));
		else messageBuffer.reset(new HoverMessage(newMessage));
		return *this;
	}

	Button& Button::setMessage(HoverMessage&& tempMessage)
	{
		if (predicatesFulfilled) message.reset(new HoverMessage(std::move(tempMessage)));
		else messageBuffer.reset(new HoverMessage(std::move(tempMessage)));
		return *this;
	}

	Button& Button::setPredicateMessage(const HoverMessage& newMessage)
	{
		predicatesFulfilled ? messageBuffer ? *messageBuffer = newMessage : messageBuffer.reset(new HoverMessage(newMessage))
			: message ? *message = newMessage : message.reset(new HoverMessage(newMessage));
		return *this;
	}

	Button& Button::setPredicateMessage(HoverMessage&& tempMessage)
	{
		predicatesFulfilled ? messageBuffer ? *messageBuffer = std::move(tempMessage) : messageBuffer.reset(new HoverMessage(std::move(tempMessage)))
			: message ? *message = std::move(tempMessage) : message.reset(new HoverMessage(std::move(tempMessage)));
		return *this;
	}

	Button& Button::setPosition(const float x, const float y)
	{
		Icon::setPosition(x, y);
		if (name) name->setPosition(x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
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

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		if (predicates && Duration(Internals::timeSinceStart() - timeOfLastPredicateCheck).count() > 1.0f / Internals::getUPS())
			arePredicatesFulfilled();

		states.shader = !predicatesFulfilled && shaderLoadSuccessful ?
			&shader : nullptr;

		target.draw(spr, states);
		if (name) target.draw(*name, states);
		Hoverable::draw(target, states);
	}

	const bool Button::arePredicatesFulfilled()const
	{
		if (predicates)
		{
			for (auto it = predicates->begin(), end = predicates->end(); it != end; ++it)
				if (!(*it)())
				{
					if (predicatesFulfilled)
					{
						spr.setColor(sf::Color(255, 255, 255, 255));
						message.swap(messageBuffer);
					}
					return predicatesFulfilled = false;
				}

			if (!predicatesFulfilled)
			{
				spr.setColor(sf::Color((1.0f - 0.15f * state) * 255, (1.0f - 0.15f * state) * 255, (1.0f - 0.15f * state) * 255, 255));
				message.swap(messageBuffer);
			}
		}
		return predicatesFulfilled = true;
	}

	const bool Button::loadShader()
	{
		if (shader.loadFromMemory(STATE_SHADER, sf::Shader::Fragment))
		{
			shader.setParameter("tex", sf::Shader::CurrentTexture);
			return true;
		}
		else return false;
	}
}