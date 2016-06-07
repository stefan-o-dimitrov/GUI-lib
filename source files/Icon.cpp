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

#include "../include/GUI/Icon.h"

namespace gui
{
	Icon::Icon(const sf::Texture& tex, const bool transparencyCheck)
	{
		setTexture(tex, transparencyCheck);
	}

	Icon::Icon(const Icon& copy)
		: Hoverable(copy), m_icon(copy.m_icon)
	{
		if (copy.m_transparency) m_transparency.reset(new TransparencyMap(*copy.m_transparency));
	}

	Icon::Icon(Icon&& temp)
		: Hoverable(std::move(temp)), m_transparency(std::move(temp.m_transparency)), m_icon(temp.m_icon) {}

	Icon& Icon::operator=(const Icon& copy)
	{
		Hoverable::operator=(copy);
		m_icon = copy.m_icon;
		if (copy.m_transparency) m_transparency.reset(new TransparencyMap(*copy.m_transparency));
	}

	Icon& Icon::operator=(Icon&& temp)
	{
		Hoverable::operator=(std::move(temp));
		m_icon = temp.m_icon;
		if (temp.m_transparency) m_transparency.reset(new TransparencyMap(std::move(*temp.m_transparency)));
	}

	std::unique_ptr<Interactive> Icon::copy() const
	{ 
		return std::unique_ptr<Icon>(new Icon(*this));
	}

	std::unique_ptr<Interactive> Icon::move()
	{ 
		return std::unique_ptr<Icon>(new Icon(std::move(*this)));
	}

	const bool Icon::contains(const float x, const float y)const
	{
		if (m_icon.getGlobalBounds().contains(x, y))
		{
			if (!m_transparency) return true;
			if (!(*m_transparency)[sf::Vector2i(x - m_icon.getPosition().x, y - m_icon.getPosition().y)]) return true;
		}
		return false;
	}

	const sf::FloatRect Icon::getGlobalBounds()const
	{
		return m_icon.getGlobalBounds();
	}

	const sf::Vector2f&  Icon::getPosition()const
	{
		return m_icon.getPosition();
	}

	const sf::Texture* Icon::getTexture()const
	{
		return m_icon.getTexture();
	}

	const bool Icon::getTransparencyCheck()const
	{
		return m_transparency != nullptr;
	}

	const sf::IntRect& Icon::getTextureRect()const
	{
		return m_icon.getTextureRect();
	}

	Icon& Icon::setPosition(const float x, const float y)
	{
		m_icon.setPosition(x, y);
		return *this;
	}

	Icon& Icon::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	Icon& Icon::setTexture(const sf::Texture& tex, const bool transparencyCheck)
	{
		m_icon.setTexture(tex);
		setTransparencyCheck(transparencyCheck);
		return *this;
	}

	Icon& Icon::setTransparencyCheck(const bool newTransparencyCheck)
	{
		newTransparencyCheck ? m_transparency.reset(new TransparencyMap(*m_icon.getTexture())) : m_transparency.reset();
		return *this;
	}

	Icon& Icon::setTextureRect(const sf::IntRect& rect)
	{
		m_icon.setTextureRect(rect);
		return *this;
	}

	Icon& Icon::setColor(const sf::Color& color)
	{
		m_icon.setColor(color);
		return *this;
	}

	Icon& Icon::clearMessage()
	{
		Hoverable::clearMessage();
		return *this;
	}

	Icon& Icon::setMessage(const HoverMessage& newMessage)
	{
		Hoverable::setMessage(newMessage);
		return *this;
	}

	Icon& Icon::setMessage(HoverMessage&& tempMessage)
	{
		Hoverable::setMessage(std::move(tempMessage));
		return *this;
	}

	void Icon::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		target.draw(m_icon, states);
		Hoverable::draw(target, states);
	}
}