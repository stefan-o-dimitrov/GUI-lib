#include "Hoverable.h"

const bool gui::Hoverable::input(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::MouseMoved:
	{
		const sf::Vector2f mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
		if (!contains(mousePos)) { mouseLeft(); return false; }
		else { if(!hasBeenPressed) mouseEntered(mousePos); return true; }
	}
	case sf::Event::MouseButtonPressed:
		mouseLeft();
		hasBeenPressed = true;
		return contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
	case sf::Event::MouseButtonReleased:
		return contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
	default:
		return false;
	}
}

const std::shared_ptr<const gui::HoverMessage> gui::Hoverable::getMessage() const
{
	return message;
}

gui::Hoverable& gui::Hoverable::clearMessage()
{
	message.reset();
	return *this;
}

gui::Hoverable& gui::Hoverable::setMessage(const HoverMessage& newMessage)
{
	message.reset(new HoverMessage(newMessage));
	return *this;
}

gui::Hoverable& gui::Hoverable::setMessage(HoverMessage&& tempMessage)
{
	message.reset(new HoverMessage(std::move(tempMessage)));
	return *this;
}

gui::Hoverable& gui::Hoverable::setDelay(const float newDelay)
{
	newDelay >= 0.0f ? delay = newDelay : delay = 0.0f;
	return *this;
}

gui::Hoverable::Hoverable(const HoverMessage& newMessage, const float newDelay)
	: message(new HoverMessage(newMessage))
{
	setDelay(newDelay);
}

gui::Hoverable::Hoverable(const Hoverable& copy)
{
	setDelay(copy.delay);
	if (copy.message) message.reset(new HoverMessage(*copy.message));
}

void gui::Hoverable::mouseEntered(const sf::Vector2f& mousePos)
{
	if(!timeMouseEntered) timeMouseEntered.reset(new float(clock.getElapsedTime().asSeconds()));
	if (message) message->setPosition(mousePos.x, mousePos.y + 32);
}

void gui::Hoverable::mouseLeft()
{
	hasBeenPressed = false;
	messageDelayPassed = false;
	timeMouseEntered.reset();
}

const bool gui::Hoverable::hasMessageDelayPassed() const
{
	return messageDelayPassed;
}

void gui::Hoverable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (timeMouseEntered && (messageDelayPassed || clock.getElapsedTime().asSeconds() - *timeMouseEntered > delay) && message)
	{
		states.shader = nullptr;
		messageDelayPassed = true;
		target.draw(*message, states);
	}
}
