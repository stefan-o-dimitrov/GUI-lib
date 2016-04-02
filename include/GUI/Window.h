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

#ifndef GUI_WINDOW
#define GUI_WINDOW

#include <vector>
#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>

#include "Interactive.h"
#include "TransparencyMap.h"

namespace gui
{
	class Window final : public sf::Drawable
	{
	public:
		Window(const Window& copy);
		Window(Window&& temp) = default;
		Window() = default;
		~Window() = default;

		const bool contains(const sf::Vector2f& point)const;
		const bool input(const sf::Event& event);
		void lostFocus();

		const sf::Vector2f& getPosition()const;
		const bool isMovable()const;

		Window& setPosition(const sf::Vector2f& position);
		Window& setPosition(const float x, const float y);
		Window& setBackgroundTexture(const sf::Texture& texture, const bool transparencyCheck = false);
		Window& setTransparencyCheck(const bool transparencyCheck);
		Window& setBackgroundTextureRect(const sf::IntRect& textureRect);
		Window& setMovable(const bool isMovable);

		Window& add(Interactive&& element);
		Window& add(const Interactive& element);

		void clear();
		const bool erase(const unsigned short index);
		const size_t size()const;

		Interactive& at(const unsigned short index);
		const Interactive& at(const unsigned short index)const;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
		
		sf::Sprite background;
		std::unique_ptr<TransparencyMap> transparency = nullptr;
		bool movable = false;
		std::unique_ptr<sf::Vector2f> mouseDragOffset = nullptr;
		std::vector<std::shared_ptr<Interactive>> elements, elementOrder;
	};
}

#endif