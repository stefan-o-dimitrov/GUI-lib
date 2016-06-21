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

#ifndef GUI_BACKGROUND
#define GUI_BACKGROUND

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>

namespace gui
{
	class Background final : public sf::Drawable
	{
	public:
		Background(const Background& copy) = default;
		Background(Background&& temp) = default;
		Background(const sf::Texture& texture, const sf::FloatRect& target = sf::FloatRect(0, 0, 1, 1));
		Background();
		~Background() = default;

		Background& operator=(const Background& copy) = default;
		Background& operator=(Background&& copy) = default;

		Background& setTexture(const sf::Texture& texture);
		Background& setTargetRect(const sf::FloatRect& target);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		sf::Sprite m_background;
		sf::View m_view;
	};
}

#endif