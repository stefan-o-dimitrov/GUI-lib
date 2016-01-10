#include "Hoverable.h"

const bool gui::Hoverable::input(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::MouseMoved:
	{
		const sf::Vector2f mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
		if (!contains(mousePos)) { mouseLeft(); return false; }
		else { mouseEntered(mousePos); return true; }
	}
	case sf::Event::MouseButtonPressed:
	{
		const sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
		mouseLeft();
		return contains(mousePos);
	}
	case sf::Event::MouseButtonReleased:
	{
		const sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
		return contains(mousePos);
	}
	default:
		return false;
	}
}

const std::shared_ptr<const gui::HoverMessage> gui::Hoverable::getMessage() const
{
	return message;
}

gui::Hoverable& gui::Hoverable::setMessage(const HoverMessage& _message)
{
	message.reset(new HoverMessage(_message));
	return *this;
}

gui::Hoverable& gui::Hoverable::setMessage(HoverMessage&& _message)
{
	message.reset(new HoverMessage((HoverMessage&&)_message));
	return *this;
}

gui::Hoverable& gui::Hoverable::setDelay(const float _delay)
{
	_delay >= 0.0f ? delay = _delay : delay = 0.0f;
	return *this;
}

gui::Hoverable::Hoverable(const HoverMessage& _message, const float _delay)
	: message(new HoverMessage(_message))
{
	setDelay(_delay);
}

gui::Hoverable::Hoverable(const Hoverable& _lVal)
{
	setDelay(_lVal.delay);
	if (_lVal.message) message.reset(new HoverMessage(*_lVal.message));
}

gui::Hoverable& gui::Hoverable::operator=(const Hoverable& _lVal)
{
	setDelay(_lVal.delay);
	if (_lVal.message) message.reset(new HoverMessage(*_lVal.message));
	return *this;
}

void gui::Hoverable::mouseEntered(const sf::Vector2f& mousePos)
{
	if(!timeMouseEntered) timeMouseEntered.reset(new float(clock.getElapsedTime().asSeconds()));
	if (message) message->setPosition(mousePos);
}

void gui::Hoverable::mouseLeft()
{
	messageDelayPassed = false;
	timeMouseEntered.reset();
}

const bool gui::Hoverable::hasMessageDelayPassed() const
{
	return messageDelayPassed;
}

void gui::Hoverable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (timeMouseEntered && (messageDelayPassed || clock.getElapsedTime().asSeconds() - *timeMouseEntered < delay) && message)
	{
		messageDelayPassed = true;
		target.draw(*message, states);
	}
}
