#pragma once

#include <SFML/Graphics.hpp>

#include "Button.hpp"

namespace gui
{
	template <typename eventFunctionFalse = void(),
		typename eventFunctionTrue = void(),
		typename predicateFunctionFalse = bool(),
		typename predicateFunctionTrue = bool()> 
	class CheckBox final : public Button<eventFunctionFalse, predicateFunctionFalse>
	{
	public:
		CheckBox(const Button<eventFunctionFalse, predicateFunctionFalse>& falseState, const Button<eventFunctionTrue, predicateFunctionTrue>& trueState, const sf::Vector2f& position = sf::Vector2f(0, 0), const bool state = false);
		CheckBox(Button<eventFunctionFalse, predicateFunctionFalse>&& falseState, Button<eventFunctionTrue, predicateFunctionTrue>&& trueState, const sf::Vector2f& position = sf::Vector2f(0, 0), const bool state = false);
		CheckBox(const CheckBox& copy) = default;
		CheckBox(CheckBox&& temp) = default;
		CheckBox() = default;
		~CheckBox() = default;

		CheckBox& operator=(const CheckBox& copy) = default;
		CheckBox& operator=(CheckBox&& temp) = default;

		const bool input(const sf::Event& event)override;
		const bool contains(const sf::Vector2f& point)const;

		const State getState()const override;
		const bool isChecked()const;

		const sf::FloatRect getTrueStateGlobalBounds()const;
		const sf::Texture& getTrueStateTexture()const;
		const bool getTrueStateTransparencyCheck()const;
		const sf::IntRect& getTrueStateTextureRect()const;
		const std::shared_ptr<const HoverMessage> getTrueStateMessage()const;
		const std::shared_ptr<const HoverMessage> getTrueStatePredicateMessage()const;
		
		void toggle();

		CheckBox& setIsChecked(const bool isChecked);
		CheckBox& setPosition(const float x, const float y)override;

		CheckBox& setTrueStateTexture(const sf::Texture& texture, const bool transparencyCheck = false);
		CheckBox& setTrueStateTransparencyCheck(const bool transparencyCheck);
		CheckBox& setTrueStateTextureRect(const sf::IntRect& textureRect);
		CheckBox& setTrueStateClickSound(const unsigned short soundKey);
		CheckBox& setTrueStateMessage(const HoverMessage& message);
		CheckBox& setTrueStateMessage(HoverMessage&& messageTemp);
		CheckBox& setTrueStatePredicates(const predicateArray& predicates, const sf::Font& font, const unsigned char characterSize = 13);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
		
		using Button<eventFunctionFalse, predicateFunctionFalse>::getName;
		using Button<eventFunctionFalse, predicateFunctionFalse>::setName;

		Button<eventFunctionTrue, predicateFunctionTrue> trueState;
		bool                                             checked = false;
	};
};


template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::CheckBox(const Button<eventFunctionFalse, predicateFunctionFalse>& _fState, const Button<eventFunctionTrue, predicateFunctionTrue>& _tState, const sf::Vector2f& _pos, const bool _state)
	: Button<eventFunctionFalse, predicateFunctionFalse>(_fState), trueState(_tState), checked(_state)
{
	if (name) name.reset();
	if (trueState.name) trueState.name.reset();
	setPosition(_pos);
}

template<typename eventFunctionFalse, typename eventFunctionTrue, typename predicateFunctionFalse, typename predicateFunctionTrue>
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::CheckBox(Button<eventFunctionFalse, predicateFunctionFalse>&& _fState, Button<eventFunctionTrue, predicateFunctionTrue>&& _tState, const sf::Vector2f & _pos, const bool _state)
	: Button<eventFunctionFalse, predicateFunctionFalse>(std::move(_tState)), trueState(std::move(_tState)), checked(_state)
{
	if (name) name.reset();
	if (trueState.name) trueState.name.reset();
	setPosition(_pos);
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
const bool gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::input(const sf::Event& event)
{
	if (checked)
	{
		if (event.type == sf::Event::MouseButtonReleased && trueState.getState() == PressedDown) checked = false;
		return trueState.input(event);
	}
	else
	{
		if (event.type == sf::Event::MouseButtonReleased && Button::getState() == PressedDown) checked = true;
		return Button<eventFunctionFalse, predicateFunctionFalse>::input(event);
	}
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
const bool gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::contains(const sf::Vector2f& _pos)const
{
	if (checked) return trueState.contains(_pos);
	else return Button<eventFunctionFalse, predicateFunctionFalse>::contains(_pos);
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
const gui::State gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::getState()const
{
	if (checked) return trueState.getState();
	else return Button<eventFunctionFalse, predicateFunctionFalse>::getState();
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
const bool gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::isChecked()const
{
	return checked;
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
const sf::FloatRect gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::getTrueStateGlobalBounds()const
{
	return trueState.getGlobalBounds();
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
const sf::Texture& gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::getTrueStateTexture()const
{
	return trueState.getTexture();
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
const bool gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::getTrueStateTransparencyCheck()const
{
	return trueState.getTransparencyCheck();
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
const sf::IntRect& gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::getTrueStateTextureRect()const
{
	return trueState.getTextureRect();
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
const std::shared_ptr<const gui::HoverMessage> gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::getTrueStateMessage()const
{
		return trueState.getMessage();
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
const std::shared_ptr<const gui::HoverMessage> gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::getTrueStatePredicateMessage()const
{
	return trueState.getPredicateMessage();
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
void gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::toggle()
{
	if (checked)
	{
		checked = false;
		trueState.onClickAction();
	}
	else
	{
		checked = true;
		onClickAction();
	}
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>&
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::setIsChecked(const bool _cVal)
{
	if (checked != _cVal)
		toggle();
	return *this;
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>&
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::setPosition(const float x, const float y)
{
	Button<eventFunctionFalse, predicateFunctionFalse>::setPosition(x, y);
	trueState.setPosition(x + getGlobalBounds().width / 2.0f - trueState.getGlobalBounds().width / 2.0f, y + getGlobalBounds().height / 2.0f - trueState.getGlobalBounds().height / 2.0f);
	return *this;
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>&
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::setTrueStateTexture(const sf::Texture& _tex, const bool _transpCheck)
{
	trueState.setTexture(_tex, _transpCheck);
	return *this;
}

template <typename eventFunctionFalse, typename eventFunctionTrue,
	typename predicateFunctionFalse, typename predicateFunctionTrue>
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>&
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::setTrueStateTransparencyCheck(const bool _transpCheck)
{
	trueState.setTransparencyCheck(_transpCheck);
	return *this;
}

template<typename eventFunctionFalse, typename eventFunctionTrue, typename predicateFunctionFalse, typename predicateFunctionTrue>
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>&
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::setTrueStateTextureRect(const sf::IntRect& _rect)
{
	trueState.setTextureRect(_rect);
	return *this;
}

template<typename eventFunctionFalse, typename eventFunctionTrue, typename predicateFunctionFalse, typename predicateFunctionTrue>
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>&
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::setTrueStateClickSound(const unsigned short _soundKey)
{
	trueState.setClickSound(_soundKey);
	return *this;
}

template<typename eventFunctionFalse, typename eventFunctionTrue, typename predicateFunctionFalse, typename predicateFunctionTrue>
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>&
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::setTrueStateMessage(const HoverMessage& _message)
{
	trueState.setMessage(_message);
	return *this;
}

template<typename eventFunctionFalse, typename eventFunctionTrue, typename predicateFunctionFalse, typename predicateFunctionTrue>
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>&
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::setTrueStateMessage(HoverMessage&& _message)
{
	trueState.setMessage((HoverMessage&&)_message);
	return *this;
}

template<typename eventFunctionFalse, typename eventFunctionTrue, typename predicateFunctionFalse, typename predicateFunctionTrue>
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>&
gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::setTrueStatePredicates(const predicateArray& _predArray, const sf::Font& font, const unsigned char charSize)
{
	trueState.setPredicates(_predArray, font, charSize);
	return *this;
}

template<typename eventFunctionFalse, typename eventFunctionTrue, typename predicateFunctionFalse, typename predicateFunctionTrue>
void gui::CheckBox<eventFunctionFalse, eventFunctionTrue, predicateFunctionFalse, predicateFunctionTrue>::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (checked) target.draw(trueState, states);
	else Button<eventFunctionFalse, predicateFunctionFalse>::draw(target, states);
}
