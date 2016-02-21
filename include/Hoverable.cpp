#include "Hoverable.h"
#include "Internals.h"

namespace gui
{
	const bool Hoverable::input(const sf::Event& event)
	{
		switch (event.type)
		{
		case sf::Event::MouseMoved:
		{
			const sf::Vector2f mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
			if (!contains(mousePos)) { mouseLeft(); return false; }
			else { if (!hasBeenPressed) mouseEntered(mousePos); return true; }
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

	const std::shared_ptr<const HoverMessage> Hoverable::getMessage() const
	{
		return message;
	}

	Hoverable& Hoverable::clearMessage()
	{
		message.reset();
		return *this;
	}

	Hoverable& Hoverable::setMessage(const HoverMessage& newMessage)
	{
		message.reset(new HoverMessage(newMessage));
		return *this;
	}

	Hoverable& Hoverable::setMessage(HoverMessage&& tempMessage)
	{
		message.reset(new HoverMessage(std::move(tempMessage)));
		return *this;
	}

	Hoverable& Hoverable::setDelay(const float newDelay)
	{
		newDelay >= 0.0f ? delay = newDelay : delay = 0.0f;
		return *this;
	}

	Hoverable::Hoverable(const HoverMessage& newMessage, const float newDelay)
		: message(new HoverMessage(newMessage))
	{
		setDelay(newDelay);
	}

	Hoverable::Hoverable(const Hoverable& copy)
	{
		setDelay(copy.delay);
		if (copy.message) message.reset(new HoverMessage(*copy.message));
	}

	Hoverable& Hoverable::operator=(const Hoverable& copy)
	{
		setDelay(copy.delay);
		if (copy.message) message.reset(new HoverMessage(*copy.message));
	}

	void Hoverable::mouseEntered(const sf::Vector2f& mousePos)
	{
		if (!timeMouseEntered) timeMouseEntered.reset(new TimePoint(Internals::timeSinceStart()));
		if (message) message->setPosition(mousePos.x, mousePos.y + 32);
	}

	void Hoverable::mouseLeft()
	{
		hasBeenPressed = false;
		messageDelayPassed = false;
		timeMouseEntered.reset();
	}

	const bool Hoverable::hasMessageDelayPassed() const
	{
		return messageDelayPassed;
	}

	void Hoverable::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (timeMouseEntered && (messageDelayPassed || Duration(Internals::timeSinceStart() - *timeMouseEntered).count() > delay) && message)
		{
			states.shader = nullptr;
			messageDelayPassed = true;
			target.draw(*message, states);
		}
	}
}
