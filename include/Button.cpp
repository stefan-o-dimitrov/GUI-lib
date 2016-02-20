#include "Button.h"

#include <sstream>
#include "AudioSystem.h"

const std::string gui::Button::STATE_SHADER =
"uniform sampler2D tex;\
\
void main()\
{\
	vec4 color = texture2D( tex, gl_TexCoord[0].xy );\
	float greyValue = color.r * 0.29 + color.g * 0.58 + color.b * 0.13;\
	gl_FragColor = vec4(greyValue, greyValue, greyValue, color.a);\
}";
const unsigned char gui::Button::PREDICATE_CHECKS_PER_SECOND = 10;

gui::Button::Button(const Icon& visual, const std::function<void()>& onClick)
	: Icon(visual), onClickAction(onClick)
{
	stateShader.loadFromMemory(STATE_SHADER, sf::Shader::Fragment);
	stateShader.setParameter("tex", sf::Shader::CurrentTexture);
}

gui::Button::Button(Icon&& visual, std::function<void()>&& onClick)
	: Icon(std::move(visual)), onClickAction(std::move(onClick))
{
	stateShader.loadFromMemory(STATE_SHADER, sf::Shader::Fragment);
	stateShader.setParameter("tex", sf::Shader::CurrentTexture);
}

gui::Button::Button(const Button& copy)
	: Icon(copy), onClickAction(copy.onClickAction)
{
	stateShader.loadFromMemory(STATE_SHADER, sf::Shader::Fragment);
	Icon::spr.setColor(sf::Color(0.75 * 255, 0.75 * 255, 0.75 * 255, 255));
	state = Idle;

	if (copy.messageBuffer) messageBuffer.reset(new HoverMessage(*copy.messageBuffer));
	if (copy.stringBuffer) stringBuffer.reset(new ColoredString(*copy.stringBuffer));
	if (copy.name) name.reset(new TextArea(*copy.name));
	if (copy.sound) sound.reset(new unsigned short(*copy.sound));
	if (copy.predicates) predicates.reset(new predicateArray(*copy.predicates));
}

const bool gui::Button::input(const sf::Event& event)
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
					if (sound) AudioSystem::playInterfaceSound(*sound);
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

const gui::State gui::Button::getState()const
{
	return state;
}

const std::shared_ptr<const gui::HoverMessage> gui::Button::getPredicateMessage()const
{
	if (predicatesFulfilled) return messageBuffer;
	else return message;
}

const std::shared_ptr<const gui::HoverMessage> gui::Button::getMessage()const
{
	if (predicatesFulfilled) return message;
	else return messageBuffer;
}

const std::shared_ptr<const gui::TextArea> gui::Button::getName()const
{
	return name;
}

const std::shared_ptr<const unsigned short> gui::Button::getClickSound()const
{
	return sound;
}

gui::Button& gui::Button::setName(const TextArea& newName)
{
	name.reset(new TextArea(newName));
	name->message.reset();
	name->setPosition(getPosition().x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, getPosition().y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
	return *this;
}

gui::Button& gui::Button::setName(TextArea&& tempName)
{
	name.reset(new TextArea(std::move(tempName)));
	name->message.reset();
	name->updateFunction.reset();
	name->setPosition(getPosition().x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, getPosition().y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
	return *this;
}

gui::Button& gui::Button::setClickSound(const unsigned short newSound)
{
	sound ? *sound = newSound : sound.reset(new unsigned short(newSound));
	return *this;
}

gui::Button& gui::Button::setMessage(const HoverMessage& newMessage)
{
	if (predicatesFulfilled) message.reset(new HoverMessage(newMessage));
	else messageBuffer.reset(new HoverMessage(newMessage));
	return *this;
}

gui::Button& gui::Button::setMessage(HoverMessage&& tempMessage)
{
	if (predicatesFulfilled) message.reset(new HoverMessage(std::move(tempMessage)));
	else messageBuffer.reset(new HoverMessage(std::move(tempMessage)));
	return *this;
}

gui::Button& gui::Button::setPredicateMessage(const HoverMessage& newMessage)
{
	predicatesFulfilled ? messageBuffer ? *messageBuffer = newMessage : messageBuffer.reset(new HoverMessage(newMessage))
		: message ? *message = newMessage : message.reset(new HoverMessage(newMessage));
}

gui::Button& gui::Button::setPredicateMessage(HoverMessage&& tempMessage)
{
	predicatesFulfilled ? messageBuffer ? *messageBuffer = std::move(tempMessage) : messageBuffer.reset(new HoverMessage(std::move(tempMessage)))
		: message ? *message = std::move(tempMessage) : message.reset(new HoverMessage(std::move(tempMessage)));
}

gui::Button& gui::Button::setPosition(const float x, const float y)
{
	Icon::setPosition(x, y);
	if (name) name->setPosition(x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
	return *this;
}

gui::Button& gui::Button::setPredicates(const predicateArray& newPredicates)
{
	predicates.reset(new predicateArray(newPredicates));
	return *this;
}

gui::Button& gui::Button::setPredicates(predicateArray&& tempPredicates)
{
	predicates.reset(new predicateArray(std::move(tempPredicates)));
	return *this;
}

void gui::Button::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (predicates && Internals::getClock().getElapsedTime().asSeconds() - timeOfLastPredicateCheck > 1.0f / PREDICATE_CHECKS_PER_SECOND)
		arePredicatesFulfilled();
	predicatesFulfilled ? 0 : states.shader = &stateShader;
	Icon::draw(target, states);
	if (name) target.draw(*name, states);
}

const bool gui::Button::arePredicatesFulfilled()const
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