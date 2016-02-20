#include "CheckBox.h"

gui::CheckBox::CheckBox(const Button& fState, const Button& tState, const sf::Vector2f& pos, const bool state)
	: Button(fState), trueState(tState), checked(state)
{
	if (name) name.reset();
	if (trueState.name) trueState.name.reset();
	setPosition(pos);
}

gui::CheckBox::CheckBox(Button&& fState, Button&& tState, const sf::Vector2f& pos, const bool state)
	: Button(std::move(fState)), trueState(std::move(tState)), checked(state)
{
	if (name) name.reset();
	if (trueState.name) trueState.name.reset();
	setPosition(pos);
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

const bool gui::CheckBox::contains(const sf::Vector2f& pos)const
{
	if (checked) return trueState.contains(pos);
	else return Button::contains(pos);
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

gui::CheckBox& gui::CheckBox::setIsChecked(const bool state)
{
	if (checked != state)
		toggle();
	return *this;
}

gui::CheckBox& gui::CheckBox::setPosition(const float x, const float y)
{
	Button::setPosition(x, y);
	trueState.setPosition(x + getGlobalBounds().width / 2.0f - trueState.getGlobalBounds().width / 2.0f, y + getGlobalBounds().height / 2.0f - trueState.getGlobalBounds().height / 2.0f);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateTexture(const sf::Texture& tex, const bool transpCheck)
{
	trueState.setTexture(tex, transpCheck);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateTransparencyCheck(const bool transpCheck)
{
	trueState.setTransparencyCheck(transpCheck);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateTextureRect(const sf::IntRect& rect)
{
	trueState.setTextureRect(rect);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateClickSound(const unsigned short sound)
{
	trueState.setClickSound(sound);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateMessage(const HoverMessage& newMessage)
{
	trueState.setMessage(newMessage);
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStateMessage(HoverMessage&& tempMessage)
{
	trueState.setMessage(std::move(tempMessage));
	return *this;
}

gui::CheckBox& gui::CheckBox::setTrueStatePredicates(const predicateArray& newPredicates)
{
	trueState.setPredicates(newPredicates);
	return *this;
}

void gui::CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (checked) target.draw(trueState, states);
	else Button::draw(target, states);
}
