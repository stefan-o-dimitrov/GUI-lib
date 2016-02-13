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

gui::Button::Button(const Button& _lVal)
	: Icon(_lVal), onClickAction(_lVal.onClickAction)
{
	stateShader.loadFromMemory(STATE_SHADER, sf::Shader::Fragment);
	Icon::spr.setColor(sf::Color(0.75 * 255, 0.75 * 255, 0.75 * 255, 255));
	state = Idle;

	if (_lVal.messageBuffer) messageBuffer.reset(new HoverMessage(*_lVal.messageBuffer));
	if (_lVal.stringBuffer) stringBuffer.reset(new ColoredString(*_lVal.stringBuffer));
	if (_lVal.name) name.reset(new TextArea(*_lVal.name));
	if (_lVal.sound) sound.reset(new unsigned short(*_lVal.sound));
	if (_lVal.predicates) predicates.reset(new predicateArray(*_lVal.predicates));
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

gui::Button& gui::Button::setName(const TextArea& _lVal)
{
	name.reset(new TextArea(_lVal));
	name->message.reset();
	name->setPosition(getPosition().x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, getPosition().y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
	return *this;
}

gui::Button& gui::Button::setName(TextArea&& _rVal)
{
	name.reset(new TextArea(std::move(_rVal)));
	name->message.reset();
	name->updateFunction.reset();
	name->setPosition(getPosition().x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, getPosition().y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
	return *this;
}

gui::Button& gui::Button::setClickSound(const unsigned short _soundKey)
{
	sound.reset(new unsigned short(_soundKey));
	return *this;
}

gui::Button& gui::Button::setMessage(const HoverMessage& _message)
{
	if (predicatesFulfilled) message.reset(new HoverMessage(_message));
	else messageBuffer.reset(new HoverMessage(_message));
	return *this;
}

gui::Button& gui::Button::setMessage(HoverMessage&& _message)
{
	if (predicatesFulfilled) message.reset(new HoverMessage(std::move(_message)));
	else messageBuffer.reset(new HoverMessage(std::move(_message)));
	return *this;
}

gui::Button& gui::Button::setPosition(const float x, const float y)
{
	Icon::setPosition(x, y);
	if (name) name->setPosition(x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
	return *this;
}

gui::Button& gui::Button::setPredicates(const predicateArray& _predArray, const sf::Font& font, const unsigned char charSize)
{
	predicates.reset(new predicateArray(_predArray));
	if (predicatesFulfilled) messageBuffer.reset(new HoverMessage(ColoredString(), font, charSize));
	else message.reset(new HoverMessage(ColoredString(), font, charSize));
	checkPredicates();
	return *this;
}

void gui::Button::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (hasMessageDelayPassed())
		checkPredicates();
	else arePredicatesFulfilled();
	predicatesFulfilled ? 0 : states.shader = &stateShader;
	Icon::draw(target, states);
	if (name) target.draw(*name, states);
}

void gui::Button::checkPredicates()const
{
	/*if (predicates && clock.getElapsedTime().asSeconds() > timeOfLastPredicateCheck + 0.04 && messageBuffer)
	{
	if (!stringBuffer) { stringBuffer.reset(new ColoredString()); stringBuffer->vec.reserve(predicates->size() * 3 + 1); }
	stringBuffer->vec.clear();
	bool overallValidity = true;
	for (auto it = predicates->begin(), end = predicates->end(); it != end; ++it)
	{
	const bool value = it->first();
	stringBuffer->vec.push_back(std::unique_ptr<cString>(new cString("(", sf::Color::White)));
	stringBuffer->vec.push_back(std::unique_ptr<cString>(new cString("*", value ? sf::Color::Green : sf::Color::Red)));
	stringBuffer->vec.push_back(std::unique_ptr<cString>(new cString(") " + it->second, sf::Color::White)));
	if (!value) overallValidity = false;
	}

	if (predicatesFulfilled)
	messageBuffer->setText(*stringBuffer);
	else
	message->setText(*stringBuffer);
	if (predicatesFulfilled != overallValidity) message.swap(messageBuffer);
	if (!overallValidity) predicatesFulfilled = false;;
	}*/
	arePredicatesFulfilled();
}

const bool gui::Button::arePredicatesFulfilled()const
{
	if (predicates)
		for (auto it = predicates->begin(), end = predicates->end(); it != end; ++it)
			if (!it->first())
			{
				if (predicatesFulfilled)
					spr.setColor(sf::Color(255, 255, 255, 255));
				return predicatesFulfilled = false;
			}
	if (!predicatesFulfilled)
		spr.setColor(sf::Color((1.0f - 0.15f * state) * 255, (1.0f - 0.15f * state) * 255, (1.0f - 0.15f * state) * 255, 255));
	return predicatesFulfilled = true;
}