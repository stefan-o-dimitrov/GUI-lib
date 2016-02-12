#pragma once

#include <memory>
#include "HoverMessage.h"
#include "Interactive.h"
#include "Internals.h"

namespace gui
{
	/// <summary>
	/// Specialised class from which all gui elements which can have a message on mouse hover shoould inherit.
	/// </summary>
	class Hoverable : public virtual Interactive
	{
	public:

		/// <summary>
		/// Allows the object to react to mouse events to determine whether or not the HoverMessage should be drawn, if set.
		/// </summary>
		virtual const bool input(const sf::Event& event)override;

		/// <summary>
		/// Checks whether or not the given point is contained within the object.
		/// </summary>
		virtual const bool contains(const sf::Vector2f& point)const = 0;

		/// <summary>
		/// Returns a read-only unique pointer to the Hover Message that the Icon uses.
		/// </summary>
		virtual const std::shared_ptr<const HoverMessage> getMessage()const;

		/// <summary>
		/// Sets the Hover Message that the Icon should display when the mouse is hovering over it, by copying the given one.
		/// </summary>
		virtual Hoverable& setMessage(const HoverMessage& message);

		/// <summary>
		/// Sets the Hover Message that the Icon should display when the mouse is hovering over it, by moving the given one.
		/// </summary>
		virtual Hoverable& setMessage(HoverMessage&& messageTemp);

		/// <summary>
		/// Sets the time to wait, in seconds, before starting to draw the HoverMessage after the mouse has entered.
		/// </summary>
		Hoverable& setDelay(const float delaySeconds);

	protected:
		
		/// <summary>
		/// Constructs the object from a given HoverMessage and an optional delay time, in seconds.
		/// </summary>
		Hoverable(const HoverMessage& message, const float delay = 0.75f);

		/// <summary>
		/// Constructs the object by copying the given one.
		/// </summary>
		Hoverable(const Hoverable& copy);

		/// <summary>
		/// Constructs the object by moving the given one.
		/// </summary>
		Hoverable(Hoverable&& temp) = default;

		/// <summary>
		/// Constructs the object without setting a HoverMessage.
		/// </summary>
		Hoverable() = default;

		/// <summary>
		/// Assigns the object by copying the given one.
		/// </summary>
		Hoverable& operator=(const Hoverable& copy);

		/// <summary>
		/// Assigns the object by moving the given one.
		/// </summary>
		Hoverable& operator=(Hoverable&& temp) = default;

		/// <summary>
		/// Sets the mouse as having entered the object.
		/// </summary>
		void mouseEntered(const sf::Vector2f& position);

		/// <summary>
		/// Sets the mouse as being outside of the object.
		/// </summary>
		void mouseLeft();

		/// <summary>
		/// Whether or not the message delay has passed.
		/// </summary>
		const bool hasMessageDelayPassed()const;

		/// <summary>
		/// Draws the HoverMessage given, if the mouse has been hovering at least the delay amount and the message is set.
		/// </summary>
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

		/// <summary>
		/// Shared pointer to the Hover Message, which should be drawn when the mouse is hovering over.
		/// </summary>
		mutable std::shared_ptr<HoverMessage> message = nullptr;

		/// <summary>
		/// Clock for the object to use to delay the mouseover message for a fixed time before starting to draw it.
		/// </summary>
		const sf::Clock& clock = Internals::getClock();
		
	private:
		
		/// <summary>
		/// Whether or not the delay has passed for the mouseover message to be drawn.
		/// </summary>
		mutable bool messageDelayPassed = false;

		/// <summary>
		/// The amount of time to wait, in seconds, after the mouse has entered before starting to draw the HoverMessage, if set.
		/// </summary>
		float delay = 0.75f;

		/// <summary>
		/// Pointer to the time at which the mouse entered, in seconds since program startup.
		/// </summary>
		std::unique_ptr<float> timeMouseEntered = nullptr;

	};
}