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
	class Window;
	class Interactive : public virtual sf::Drawable
	{
	public:
		virtual ~Interactive() = default;

		virtual std::unique_ptr<Interactive> copy()const = 0;
		virtual std::unique_ptr<Interactive> move() = 0;

		virtual void lostFocus();
		virtual const bool input(const sf::Event& event);

		virtual const bool contains(const float x, const float y)const;
		const bool contains(const sf::Vector2f& point)const;

		virtual const sf::FloatRect getGlobalBounds() const = 0;
		virtual const sf::Vector2f& getPosition() const = 0;

		virtual Interactive& setPosition(const float x, const float y) = 0;
		virtual Interactive& setPosition(const sf::Vector2f& position);

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	};
};

#endif