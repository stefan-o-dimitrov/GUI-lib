#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "Internals.h"

namespace gui 
{
	/// <summary>
	/// Abstract class from which all interactive gui elements should inherit.
	/// </summary>
	class Interactive : public sf::Drawable
	{
	public:
		/// <summary>
		/// Virtual destructor.
		/// </summary>
		virtual ~Interactive() {};

		/// <summary>
		/// Allows the GUI element to react to user interface events accordingly.
		/// </summary>
		virtual const bool input(const sf::Event& event) = 0;

		/// <summary>
		/// Returns the smallest, axis-aligned bounding rectangle of the object, in screen coordinates.
		/// </summary>
		virtual const sf::FloatRect getGlobalBounds(void) const = 0;

		/// <summary>
		/// Returns the position of the object, as its most top-left pixel, in screen coordinates.
		/// </summary>
		virtual const sf::Vector2f& getPosition() const = 0;

		/// <summary>
		/// Sets the position of the object, as its most top-left pixel, in screen coordinates.
		/// </summary>
		virtual Drawable& setPosition(const float x, const float y) = 0;

		/// <summary>
		/// Sets the position of the object, as its most top-left pixel, in screen coordinates.
		/// </summary>
		Drawable& setPosition(const sf::Vector2f& position) { return setPosition(position.x, position.y); };

	protected:
		/// <summary>
		/// Draws the object on a given render target with the given render states.
		/// </summary>
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	};
};