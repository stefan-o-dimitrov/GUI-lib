#include "CheckBox.h"

gui::CheckBox::CheckBox(const Button& _fState, const Button& _tState, const sf::Vector2f& _pos, const bool _state)
	: Button(_fState), trueState(_tState), checked(_state)
{
	if (name) name.reset();
	if (trueState.name) trueState.name.reset();
	setPosition(_pos);
}

gui::CheckBox::CheckBox(Button&& _fState, Button&& _tState, const sf::Vector2f & _pos, const bool _state)
	: Button(std::move(_tState)), trueState(std::move(_tState)), checked(_state)
{
	if (name) name.reset();
	if (trueState.name) trueState.name.reset();
	setPosition(_pos);
}

const bool gui::CheckBox::input(const sf::Event& event)
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

const bool gui::CheckBox::contains(const sf::Vector2f& _pos)const
{
	if (checked) return trueState.contains(_pos);
	else return Button::contains(_pos);
}

const gui::State gui::CheckBox::getState()const
{
	if (checked) return trueState.getState();
	else return Button::getState();
}

const bool gui::CheckBox::isChecked()const
{
	return checked;
}

const sf::FloatRect gui::CheckBox::getTrueStateGlobalBounds()const
{
	return trueState.getGlobalBounds();
}

const sf::Texture& gui::CheckBox::getTrueStateTexture()const
{
	return trueState.getTexture();
}

const bool gui::CheckBox::getTrueStateTransparencyCheck()const
{
	return trueState.getTransparencyCheck();
}

const sf::IntRect& gui::CheckBox::getTrueStateTextureRect()const
{
	return trueState.getTextureRect();
}

const std::shared_ptr<const gui::HoverMessage> gui::CheckBox::getTrueStateMessage()const
{
	return trueState.getMessage();
}

const std::shared_ptr<const gui::HoverMessage> gui::CheckBox::getTrueStatePredicateMessage()const
{
	return trueState.getPredicateMessage();
}

void gui::CheckBox::toggle()
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

gui::CheckBox& gui::CheckBox::setIsChecked(const bool _cVal)
{
	if (checked != _cVal)
		toggle();
	return *this;
}

gui::CheckBox& gui::CheckBox::setPosition(const float x, const float y)
{
	Button::setPosition(x, y);
	trueState.setPosition(x + getGlobalBounds().width / 2.0f - trueState.getGlobalBounds().width / 2.0f, y + getGlobalBounds().height / 2.0f - trueState.getGlobalBounds().height / 2.0f);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateTexture(const sf::Texture& _tex, const bool _transpCheck)
{
	trueState.setTexture(_tex, _transpCheck);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateTransparencyCheck(const bool _transpCheck)
{
	trueState.setTransparencyCheck(_transpCheck);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateTextureRect(const sf::IntRect& _rect)
{
	trueState.setTextureRect(_rect);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateClickSound(const unsigned short _soundKey)
{
	trueState.setClickSound(_soundKey);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateMessage(const HoverMessage& _message)
{
	trueState.setMessage(_message);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateMessage(HoverMessage&& _message)
{
	trueState.setMessage((HoverMessage&&)_message);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStatePredicates(const predicateArray& _predArray, const sf::Font& font, const unsigned char charSize)
{
	trueState.setPredicates(_predArray, font, charSize);
	return *this;
}

void gui::CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (checked) target.draw(trueState, states);
	else Button::draw(target, states);
}
