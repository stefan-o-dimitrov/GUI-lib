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

#include "../include/GUI/TextPane.h"
#include "../include/GUI/HoverMessage.h"
#include "../include/GUI/Internals.h"

namespace gui
{
	TextPane::TextPane(const ColoredText& newString, const sf::Font& newFont, const unsigned char newCharacterSize)
		: m_characterSize(newCharacterSize), m_string(newString)
	{
		setFont(newFont);
		update();
	}

	TextPane::TextPane(ColoredText&& newString, const sf::Font& newFont, const unsigned char newCharacterSize)
		: m_characterSize(newCharacterSize), m_string(std::move(newString))
	{
		setFont(newFont);
		update();
	}

	TextPane::TextPane(const TextPane& copy)
		: m_position(copy.m_position), m_string(copy.m_string), m_font(copy.m_font)
	{
		for (auto it = copy.m_text.begin(), end = copy.m_text.end(); it != end; ++it)
			m_text.push_back(std::unique_ptr<sf::Text>(new sf::Text(*(*it))));
	}

	TextPane& TextPane::operator=(const TextPane& copy)
	{
		m_position = copy.m_position;
		m_string = copy.m_string;
		m_font = copy.m_font;
		for (auto it = copy.m_text.begin(), end = copy.m_text.end(); it != end; ++it)
			m_text.push_back(std::unique_ptr<sf::Text>(new sf::Text(*(*it))));
		return *this;
	}

	std::unique_ptr<Interactive> TextPane::copy() const
	{
		return std::unique_ptr<TextPane>(new TextPane(*this));
	}

	std::unique_ptr<Interactive> TextPane::move()
	{ 
		return std::unique_ptr<TextPane>(new TextPane(std::move(*this)));
	}

	const bool TextPane::contains(const float x, const float y) const
	{
		for (auto it = m_text.begin(), end = m_text.end(); it != end; ++it)
			if ((*it)->getGlobalBounds().contains(x, y)) return true;
		return false;
	}

	const sf::FloatRect TextPane::getGlobalBounds()const
	{
		sf::FloatRect returnValue(0, 0, 0, 0);
		for (auto it = m_text.begin(), end = m_text.end(); it != end; ++it)
		{
			if (returnValue.width < (*it)->getGlobalBounds().width + (*it)->getPosition().x)
				returnValue.width = (*it)->getGlobalBounds().width + (*it)->getPosition().x;
			if (returnValue.height < (*it)->getGlobalBounds().height + (*it)->getPosition().y)
				returnValue.height = (*it)->getGlobalBounds().height + (*it)->getPosition().y;
		}
		returnValue.left = m_position.x;
		returnValue.top = m_position.y;

		return returnValue;
	}

	const ColoredText& TextPane::getText() const
	{
		return m_string;
	}

	ColoredText& TextPane::getText()
	{
		return m_string;
	}

	const sf::Vector2f& TextPane::getPosition()const
	{
		return m_position;
	}

	const sf::Font* const TextPane::getFont()const
	{
		return m_font;
	}

	const unsigned char TextPane::getCharacterSize()const
	{
		return m_characterSize;
	}

	TextPane& TextPane::setPosition(const float x, const float y)
	{
		m_position.x = x;
		m_position.y = y;
		return *this;
	}

	TextPane& TextPane::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	TextPane& TextPane::setText(const ColoredText& newText)
	{
		m_string = newText;
		update();
		return *this;
	}

	TextPane& TextPane::setText(ColoredText&& newText)
	{
		m_string = std::move(newText);
		update();
		return *this;
	}

	TextPane& TextPane::setFont(const sf::Font& newFont)
	{
		m_font = &newFont;
		for (auto it = m_text.begin(), end = m_text.end(); it != end; ++it)
			(*it)->setFont(*m_font);
		return *this;
	}

	TextPane& TextPane::setCharacterSize(const unsigned char newCharacterSize)
	{
		m_characterSize = newCharacterSize;
		update();
		return *this;
	}

	void TextPane::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		if (m_string.isVolatile() && Duration(Internals::timeSinceStart() - m_timeOfLastUpdate).count() > 1.0f / Internals::getUPS())
			update();
		states.transform.translate(m_position);
		for (auto it = m_text.begin(), end = m_text.end(); it != end; ++it)
			target.draw(**it, states);
	}

	void TextPane::update() const
	{
		if (!m_font) return;
		m_string.getText(m_text, *m_font, m_characterSize);
		m_timeOfLastUpdate = Internals::timeSinceStart();
	}
}