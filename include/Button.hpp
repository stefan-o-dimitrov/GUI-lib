#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "Internals.h"
#include "Interactive.h"
#include "TextArea.h"
#include "Icon.h"

namespace gui 
{
	enum State
	{
		Idle,
		Hot,
		PressedDown,
		Unavailable
	};

	template <typename eventFunction = void(), typename predicateFunction = bool()> class Button : public Icon
	{
		static_assert(std::is_function<eventFunction>::value && std::is_function<predicateFunction>::value, "Template arguments must be function signatures.");

		template <typename evFuncFalse, typename evFuncTrue, typename predFuncFalse, typename predFuncTrue>
		friend class CheckBox;
	public:
		typedef std::vector<std::pair<std::function<predicateFunction>, std::string>>(predicateArray);

		Button(const Icon& visual, const std::function<eventFunction>& onClick);
		Button(Icon&& visual, std::function<eventFunction>&& onClick);
		Button(const Button& copy);
		Button(Button&& temp) = default;
		Button() = default;
		~Button() = default;

		Button<eventFunction, predicateFunction>& operator=(const Button& copy);
		Button<eventFunction, predicateFunction>& operator=(Button&& temp) = default;

		virtual const bool input(const sf::Event& event)override;

		virtual const State getState()const;
		const std::shared_ptr<const HoverMessage> getPredicateMessage()const;
		const std::shared_ptr<const HoverMessage> getMessage()const override;
		const std::shared_ptr<const sf::Text> getName()const;
		const std::shared_ptr<const unsigned short> getClickSound()const;

		Button& setName(const TextArea& name);
		Button& setName(TextArea&& nameTemp);
		Button& setClickSound(const unsigned short soundKey);
		Button& setMessage(const HoverMessage& message)override;
		Button& setMessage(HoverMessage&& messageTemp)override;
		virtual Button& setPosition(const float x, const float y)override;
		Button& setPredicates(const predicateArray& predicates, const sf::Font& font, const unsigned char characterSize = 13);

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

	private:
		void checkPredicates()const;
		const bool arePredicatesFulfilled()const;

		mutable std::shared_ptr<HoverMessage>  messageBuffer = nullptr;
		mutable std::unique_ptr<ColoredString> stringBuffer = nullptr;
		std::unique_ptr<predicateArray>        predicates = nullptr;
		std::shared_ptr<unsigned short>        sound = nullptr;
		std::shared_ptr<TextArea>              name = nullptr;

		mutable sf::Shader                     stateShader;
		std::function<eventFunction>           onClickAction;
		float                                  timeOfLastPredicateCheck = 0.0f;
		mutable State                          state = Idle;
		
		static const std::string STATE_SHADER;
	};
};

#include <sstream>

#include "AudioSystem.h"

template <typename eventFunction, typename predicateFunction>
const std::string gui::Button<eventFunction, predicateFunction>::STATE_SHADER =
"uniform sampler2D tex;\
uniform int buttonState;\
\
void main()\
{\
    if( buttonState == 0 )\
	{\
		gl_FragColor = texture2D( tex, gl_TexCoord[0].xy ) * vec4(0.75, 0.75, 0.75, 1.0);\
	}\
	else if( buttonState == 1)\
	{\
		gl_FragColor = texture2D( tex, gl_TexCoord[0].xy );\
	}\
	else if( buttonState == 2 )\
	{\
		gl_FragColor = texture2D( tex, gl_TexCoord[0].xy ) * vec4(0.5, 0.5, 0.5, 1.0);\
	}\
	else if( buttonState == 3 )\
	{\
		vec4 color = texture2D( tex, gl_TexCoord[0].xy );\
		float greyValue = color.rgb * vec4(0.29, 0.58, 0.13, 1.0);\
		gl_FragColor = vec4(greyValue, greyValue, greyValue, color.a);\
	}\
}";

template <typename eventFunction, typename predicateFunction>
gui::Button<eventFunction, predicateFunction>::Button(const Icon& visual, const std::function<eventFunction>& onClick)
	: Icon(visual), onClickAction(onClick)
{
	stateShader.loadFromMemory(STATE_SHADER, sf::Shader::Fragment);
	stateShader.setParameter("tex", sf::Shader::CurrentTexture);
}

template<typename eventFunction, typename predicateFunction>
gui::Button<eventFunction, predicateFunction>::Button(Icon&& visual, std::function<eventFunction>&& onClick)
	: Icon(std::move(visual)), onClickAction(std::move(onClick))
{
	stateShader.loadFromMemory(STATE_SHADER, sf::Shader::Fragment);
	stateShader.setParameter("tex", sf::Shader::CurrentTexture);
}

template <typename eventFunction, typename predicateFunction>
gui::Button<eventFunction, predicateFunction>::Button(const Button& _lVal)
	: Icon(_lVal), onClickAction(_lVal.onClickAction)
{
	stateShader.loadFromMemory(STATE_SHADER, sf::Shader::Fragment);
	stateShader.setParameter("buttonState", Idle);

	if (_lVal.messageBuffer) messageBuffer.reset(new HoverMessage(*_lVal.messageBuffer));
	if (_lVal.stringBuffer) stringBuffer.reset(new ColoredString(*_lVal.stringBuffer));
	if (_lVal.name) name.reset(new TextArea(*_lVal.name));
	if (_lVal.sound) sound.reset(new unsigned short(*_lVal.sound));
	if (_lVal.predicates) predicates.reset(new predicateArray(*_lVal.predicates));	
}

template <typename eventFunction, typename predicateFunction>
gui::Button<eventFunction, predicateFunction>& gui::Button<eventFunction, predicateFunction>::operator=(const Button& _lVal)
{
	Icon::operator=(_lVal);

	onClickAction = _lVal.onClickAction;

	if (_lVal.messageBuffer) messageBuffer.reset(new HoverMessage(*_lVal.messageBuffer));
	if (_lVal.stringBuffer) stringBuffer.reset(new ColoredString(*_lVal.stringBuffer));
	if (_lVal.name) name.reset(new TextArea(*_lVal.name));
	if (_lVal.sound) sound.reset(new unsigned short(*_lVal.sound));
	if (_lVal.predicates) predicates.reset(new predicateArray(*_lVal.predicates));
}

template <typename eventFunction, typename predicateFunction>
const bool gui::Button<eventFunction, predicateFunction>::input(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::MouseMoved:
		if (Icon::input(event))
		{
			if(state != Unavailable) stateShader.setParameter("buttonState", state = Hot);
			return true;
		}
		else
		{
			if (state != Unavailable) stateShader.setParameter("buttonState", state = Idle);
			return false;
		}
	case sf::Event::MouseButtonPressed:
		if (Icon::input(event))
		{
			if (state != PressedDown && state != Unavailable) stateShader.setParameter("buttonState", state = PressedDown);
			return true;
		}
		else return false;
	case sf::Event::MouseButtonReleased:
		if (Icon::input(event))
		{
			if(state != Unavailable)
			{
				if (state == PressedDown)
				{
					if (sound) AudioSystem::playInterfaceSound(*sound);
					onClickAction();
				}
				stateShader.setParameter("buttonState", state = Hot);
			}
			return true;
		}
		else return false;
	default:
		return false;
	}
}

template <typename eventFunction, typename predicateFunction>
const gui::State gui::Button<eventFunction, predicateFunction>::getState()const
{
	return state;
}

template <typename eventFunction, typename predicateFunction>
const std::shared_ptr<const gui::HoverMessage> gui::Button<eventFunction, predicateFunction>::getPredicateMessage()const
{
	if (state != Unavailable) return messageBuffer;
	else return message;
}

template <typename eventFunction, typename predicateFunction>
const std::shared_ptr<const gui::HoverMessage> gui::Button<eventFunction, predicateFunction>::getMessage()const
{
	if (state != Unavailable) return message;
	else return messageBuffer;
}

template <typename eventFunction, typename predicateFunction>
const std::shared_ptr<const sf::Text> gui::Button<eventFunction, predicateFunction>::getName()const
{
	return name;
}

template <typename eventFunction, typename predicateFunction>
const std::shared_ptr<const unsigned short> gui::Button<eventFunction, predicateFunction>::getClickSound()const
{
	return sound;
}

template<typename eventFunction, typename predicateFunction>
gui::Button<eventFunction, predicateFunction>& gui::Button<eventFunction, predicateFunction>::setName(const TextArea& _lVal)
{
	name.reset(new TextArea(_lVal));
	name->message.reset();
	name->setPosition(x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
}

template<typename eventFunction, typename predicateFunction>
gui::Button<eventFunction, predicateFunction>& gui::Button<eventFunction, predicateFunction>::setName(TextArea&& _rVal)
{
	name.reset(new TextArea(std::move(_rVal)));
	name->message.reset();
	name->setPosition(x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
}

template <typename eventFunction, typename predicateFunction>
gui::Button<eventFunction, predicateFunction>& gui::Button<eventFunction, predicateFunction>::setClickSound(const unsigned short _soundKey)
{
	sound.reset(new unsigned short(_soundKey));
	return *this;
}

template <typename eventFunction, typename predicateFunction>
gui::Button<eventFunction, predicateFunction>& gui::Button<eventFunction, predicateFunction>::setMessage(const HoverMessage& _message)
{
	if (state != Unavailable) message.reset(new HoverMessage(_message));
	else messageBuffer.reset(new HoverMessage(_message));
	return *this;
}

template<typename eventFunction, typename predicateFunction>
gui::Button<eventFunction, predicateFunction> & gui::Button<eventFunction, predicateFunction>::setMessage(HoverMessage&& _message)
{
	if (state != Unavailable) message.reset(new HoverMessage(std::move(_message)));
	else messageBuffer.reset(new HoverMessage(std::move(_message)));
	return *this;
}

template <typename eventFunction, typename predicateFunction>
gui::Button<eventFunction, predicateFunction>& gui::Button<eventFunction, predicateFunction>::setPosition(const float x, const float y)
{
	Icon::setPosition(x, y);
	if (name) name->setPosition(x + getGlobalBounds().width / 2 - name->getGlobalBounds().width / 2, y + getGlobalBounds().height / 2 - name->getGlobalBounds().height / 2);
	return *this;
}

template <typename eventFunction, typename predicateFunction>
gui::Button<eventFunction, predicateFunction>& gui::Button<eventFunction, predicateFunction>::setPredicates(const predicateArray& _predArray, const sf::Font& font, const unsigned char charSize)
{
	predicates.reset(new predicateArray(_predArray));
	if (state != Unavailable) messageBuffer.reset(new HoverMessage(ColoredString(), font, charSize));
	else message.reset(new HoverMessage(ColoredString(), font, charSize));
	checkPredicates();
	return *this;
}

template <typename eventFunction, typename predicateFunction>
void gui::Button<eventFunction, predicateFunction>::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (hasMessageDelayPassed())
		checkPredicates();
	else if (!arePredicatesFulfilled())
		stateShader.setParameter("buttonState", state = Unavailable);
	states.shader = &stateShader;
	Icon::draw(target, states);
	if (name) target.draw(*name, &stateShader);
}

template <typename eventFunction, typename predicateFunction>
void gui::Button<eventFunction, predicateFunction>::checkPredicates()const
{
	if (predicates && clock.getElapsedTime().asSeconds() > timeOfLastPredicateCheck + 0.04 && messageBuffer)
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

		if (state != Unavailable)
			messageBuffer->setText(*stringBuffer);
		else
			message->setText(*stringBuffer);
		if ((state != Unavailable) != overallValidity) message.swap(messageBuffer);
		if (!overallValidity) state = Unavailable;
	}
}

template <typename eventFunction, typename predicateFunction>
const bool gui::Button<eventFunction, predicateFunction>::arePredicatesFulfilled()const
{
	if(predicates)
		for (auto it = predicates->begin(), end = predicates->end(); it != end; ++it)
			if (!it->first()) return false;
	return true;
}

