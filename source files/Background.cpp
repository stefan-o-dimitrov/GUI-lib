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

#include "../include/GUI/Background.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace gui
{
	Background::Background(const sf::Texture& texture, const sf::FloatRect& target)
	{
		setTexture(texture);
		setTargetRect(target);
	}

	Background::Background()
	{
		setTargetRect(sf::FloatRect(0, 0, 1, 1));
	}

	Background& Background::setTexture(const sf::Texture& texture)
	{
		m_background.setTexture(texture);
		m_view.setSize(m_background.getLocalBounds().width, m_background.getLocalBounds().height);
		m_view.setCenter(m_background.getLocalBounds().width / 2, m_background.getLocalBounds().height / 2);
		return *this;
	}

	Background& Background::setTargetRect(const sf::FloatRect& target)
	{
		m_view.setViewport(target);
		return *this;
	}

	void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		const auto prev(target.getView());
		target.setView(m_view);
		
		target.draw(m_background, states);
		
		target.setView(prev);
	}
}