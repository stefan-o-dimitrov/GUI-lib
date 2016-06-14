////////////////////////////////////////////////////////////
//
// GUI-lib - Graphical User Interface Library
// Copyright (C) 2014-2016 ShtGames (shtgamessts@gmail.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef GUI_INTERACTIVE
#define GUI_INTERACTIVE

#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>

namespace gui 
{
	/// <summary>
	/// Specialised class from which every GUI element should inherit.
	/// </summary>
	class Interactive : public virtual sf::Drawable
	{
		friend class Window;
	public:
		Interactive(const Interactive& copy) = default;
		Interactive(Interactive&& temp) = default;
		Interactive() = default;
		virtual ~Interactive() = default;

		Interactive& operator=(const Interactive& copy) = default;
		Interactive& operator=(Interactive&& temp) = default;

		/// <summary>
		/// Virtual copy method; must be overriden for derived class to work correctly with gui::Window.
		/// </summary>
		virtual std::unique_ptr<Interactive> copy()const = 0;
		/// <summary>
		/// Virtual move method; must be overriden for derived class to work correctly with gui::Window.
		/// </summary>
		virtual std::unique_ptr<Interactive> move() = 0;

		/// <summary>
		/// Called when the gui::Window or sf::RenderWindow in which the object is loses focus; must be done manually if object is not placed in gui::Window.
		/// </summary>
		virtual void lostFocus();
		/// <summary>
		/// Allows the object to react to events.
		/// </summary>
		/// <param name = "event">
		/// The event that needs to be processed.
		/// </param>
		/// <returns>
		/// Whether or not the object has used the event given.
		/// </returns>
		virtual const bool input(const sf::Event& event);

		/// <summary>
		/// Whether the object contains the given point.
		/// </summary>
		/// <param name = "x">
		/// X coordinate of the world point.
		/// </param>
		/// <param name = "y">
		/// Y coordinate of the world point.
		/// </param>
		/// <returns>
		/// Whether or not the point is within the object.
		/// </returns>
		virtual const bool contains(const float x, const float y)const;
		/// <summary>
		/// Whether the object contains the given point, in world-coordinates.
		/// </summary>
		/// <param name = "point">
		/// The point in world coordinates.
		/// </param>
		/// <returns>
		/// Whether or not the point is within the object.
		/// </returns>
		const bool contains(const sf::Vector2f& point)const;

		/// <returns>
		/// Rectangle with the object's position and size.
		/// </returns>
		virtual const sf::FloatRect getGlobalBounds() const = 0;
		/// <returns>
		/// Position of the object.
		/// </returns>
		virtual const sf::Vector2f& getPosition() const = 0;

		/// <param name = "x">
		/// X coordinate of the new position.
		/// </param>
		/// <param name = "y">
		/// Y coordinate of the new position.
		/// </param>
		/// <returns>
		/// The object.
		/// </returns>
		virtual Interactive& setPosition(const float x, const float y) = 0;
		/// <param name = "position">
		/// World coordinates of the new position.
		/// </param>
		/// <returns>
		/// The object.
		/// </returns>
		virtual Interactive& setPosition(const sf::Vector2f& position);

		/// <summary>
		/// Parent gui::Window of the object; i.e. the one in which the object has been .added.
		/// </summary>
		/// <returns>
		/// Pointer to the parent; nullptr if object is not in any gui::Window.
		/// </returns>
		const Window* const getParent()const;
		/// <summary>
		/// Sets the object's parent gui::Window to the same one as the given object.
		/// </summary>
		/// <param name = "source">
		/// Object from which to set parent.
		/// </param>
		void setParentToSame(const Interactive& source);

	protected:
		/// <summary>
		/// Draws the object on the given target with the given states.
		/// </summary>
		/// <param name = "target">
		/// Target on which to draw the object.
		/// </param>
		/// <param name = "states">
		/// States with which to draw the object.
		/// </param>
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
		/// <summary>
		/// Sets this entity's parent Window to the one given.
		/// </summary>
		virtual void setParent(const Window* const parent);

		/// <summary>
		/// Pointer to the object's parent gui::Window.
		/// </summary>
		Window* m_parent = nullptr;
	};
};

#endif