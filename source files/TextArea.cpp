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

#include "../include/GUI/TextArea.h"
#include "../include/GUI/Internals.h"

#include <sstream>

namespace gui
{
	TextArea::TextArea(const std::string& newText, const sf::Font& newFont, const unsigned char newCharacterSize)
		: m_text(newText, newFont, newCharacterSize) {}

	TextArea::TextArea(const TextArea& copy)
		: Hoverable(copy), m_text(copy.m_text),
		m_updateFunction(copy.m_updateFunction) {}

	std::unique_ptr<Interactive> TextArea::copy() const
	{
		return std::unique_ptr<TextArea>(new TextArea(*this));
	}

	std::unique_ptr<Interactive> TextArea::move()
	{ 
		return std::unique_ptr<TextArea>(new TextArea(std::move(*this)));
	}

	const bool TextArea::contains(const float x, const float y) const
	{
		return m_text.getGlobalBounds().contains(x, y);
	}

	const sf::FloatRect TextArea::getGlobalBounds()const
	{
		return m_text.getGlobalBounds();
	}

	const sf::Vector2f& TextArea::getPosition()const
	{
		return m_text.getPosition();
	}

	const sf::Font& TextArea::getFont()const
	{
		return *m_text.getFont();
	}

	const unsigned char TextArea::getCharacterSize()const
	{
		return m_text.getCharacterSize();
	}

	const sf::Color& TextArea::getColor() const
	{
		return m_text.getColor();
	}

	TextArea& TextArea::clearMessage()
	{
		Hoverable::clearMessage();
		return *this;
	}

	TextArea& TextArea::setMessage(const HoverMessage& newMessage)
	{
		Hoverable::setMessage(newMessage);
		return *this;
	}

	TextArea& TextArea::setMessage(HoverMessage&& tempMessage)
	{
		Hoverable::setMessage(std::move(tempMessage));
		return *this;
	}

	TextArea& TextArea::setPosition(const float x, const float y)
	{
		m_text.setPosition(x, y);
		return *this;
	}

	TextArea& TextArea::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	TextArea& TextArea::setText(const ColoredString& newText)const
	{
		m_text.setString(newText.first);
		m_text.setColor(newText.second);
		return (TextArea&)*this;
	}

	TextArea& TextArea::setFont(const sf::Font& newFont)
	{
		m_text.setFont(newFont);
		return *this;
	}

	TextArea& TextArea::setCharacterSize(const unsigned char newCharacterSize)
	{
		m_text.setCharacterSize(newCharacterSize);
		return *this;
	}

	TextArea& TextArea::setColor(const sf::Color& color)
	{ 
		m_text.setColor(color);
		return *this;
	}

	TextArea& TextArea::setUpdateFunction(const std::function<ColoredString()>& func)
	{
		m_updateFunction.reset(new std::function<ColoredString()>(func));
		return *this;
	}

	TextArea& TextArea::setUpdateFunction(std::function<ColoredString()>&& func)
	{ 
		m_updateFunction.reset(new std::function<ColoredString()>(std::move(func)));
		return *this;
	}

	void TextArea::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		if (m_updateFunction && Duration(Internals::timeSinceStart() - m_timeOfLastUpdate).count() > 1.0f / Internals::getUPS())
		{
			setText((*m_updateFunction)());
			m_timeOfLastUpdate = Internals::timeSinceStart();
		}
		target.draw(m_text, states);
		Hoverable::draw(target, states);
	}
}