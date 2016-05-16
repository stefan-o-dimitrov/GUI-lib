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

#include "../include/GUI/HoverMessage.h"
#include "../include/GUI/Internals.h"

namespace gui
{
	const unsigned char HoverMessage::TEXT_BOX_X_SPACING = 10, HoverMessage::TEXT_BOX_Y_SPACING = 10;

	HoverMessage::HoverMessage(const ColoredText& str, const sf::Font& font, const unsigned char characterSize)
		: TextPane(str, font, characterSize)
	{
		m_textBox.setFillColor(sf::Color(0, 0, 0, 255));
		update();
	}

	HoverMessage::HoverMessage(ColoredText&& str, const sf::Font& font, const unsigned char characterSize)
		: TextPane(std::move(str), font, characterSize)
	{
		m_textBox.setFillColor(sf::Color(0, 0, 0, 255));
		update();
	}

	HoverMessage::HoverMessage(const HoverMessage& copy)
		: TextPane(copy), FadeAnimation(copy), m_textBox(copy.m_textBox) {}

	HoverMessage::HoverMessage(HoverMessage&& temp)
		: TextPane(std::move(temp)), FadeAnimation(std::move(temp)), m_textBox(temp.m_textBox) {}

	HoverMessage& HoverMessage::operator=(const HoverMessage& copy)
	{
		TextPane::operator=(copy);
		m_textBox = copy.m_textBox;
		return *this;
	}

	HoverMessage& HoverMessage::operator=(HoverMessage&& temp)
	{
		TextPane::operator=(std::move(temp));
		m_textBox = temp.m_textBox;
		return *this;
	}

	const sf::FloatRect HoverMessage::getGlobalBounds()const
	{
		return m_textBox.getGlobalBounds();
	}

	const sf::Color& HoverMessage::getBackgroundFill()const
	{
		return m_textBox.getFillColor();
	}

	const sf::Color& HoverMessage::getBorderFill()const
	{
		return m_textBox.getOutlineColor();
	}

	const char HoverMessage::getBorderThickness()const
	{
		return m_textBox.getOutlineThickness();
	}

	const ColoredText& HoverMessage::getText() const
	{
		return TextPane::getText();
	}

	ColoredText& HoverMessage::getText()
	{
		return TextPane::getText();
	}
	
	void HoverMessage::updateBox()const
	{
		const sf::FloatRect rect = TextPane::getGlobalBounds();
		m_textBox.setSize(sf::Vector2f(rect.width + TEXT_BOX_X_SPACING * 2, rect.height + TEXT_BOX_Y_SPACING * 2));
	}

	void HoverMessage::update() const
	{
		TextPane::update();
		updateBox();
	}

	const sf::Vector2f& HoverMessage::getPosition()const
	{
		return m_position;
	}

	const sf::Font& HoverMessage::getFont() const
	{
		return TextPane::getFont();
	}

	const unsigned char HoverMessage::getCharacterSize() const
	{
		return TextPane::getCharacterSize();
	}

	HoverMessage& HoverMessage::setPosition(const float x, const float y)
	{
		m_position.x = x;
		m_position.y = y;
		m_textBox.setPosition(m_position);
		return *this;
	}

	HoverMessage& HoverMessage::setPosition(const sf::Vector2f& pos)
	{
		return setPosition(pos.x, pos.y);
	}

	HoverMessage& HoverMessage::setText(const ColoredText& newText)
	{
		TextPane::setText(newText);
		return *this;
	}

	HoverMessage& HoverMessage::setText(ColoredText&& newText)
	{
		TextPane::setText(std::move(newText));
		return *this;
	}

	HoverMessage& HoverMessage::setFont(const sf::Font& newFont)
	{
		TextPane::setFont(newFont);
		return *this;
	}

	HoverMessage& HoverMessage::setCharacterSize(const unsigned char newCharacterSize)
	{
		TextPane::setCharacterSize(newCharacterSize);
		return *this;
	}

	HoverMessage& HoverMessage::setFadeDirection(const bool direction)
	{
		FadeAnimation::setFadeDirection(direction);
		return *this;
	}

	HoverMessage& HoverMessage::setDuration(const float duration)
	{
		FadeAnimation::setDuration(duration);
		return *this;
	}
		
	HoverMessage& HoverMessage::setBackgroundFill(const sf::Color& color)
	{
		m_textBox.setFillColor(color);
		return *this;
	}

	HoverMessage& HoverMessage::setBorderFill(const sf::Color& color)
	{
		m_textBox.setOutlineColor(color);
		return *this;
	}

	HoverMessage& HoverMessage::setBorderThickness(const char thickness)
	{
		m_textBox.setOutlineThickness(thickness);
		return *this;
	}
	
	void HoverMessage::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		FadeAnimation::draw(target, states);
		if (FadeAnimation::getFadeAmount() == 0.0f) return;

		states.transform.translate(sf::Vector2f(m_position.x + m_textBox.getGlobalBounds().width > target.getSize().x ?
			(target.getSize().x - m_textBox.getGlobalBounds().width) - m_position.x : 0,
			m_position.y + m_textBox.getGlobalBounds().height > target.getSize().y ?
			(target.getSize().y - m_textBox.getGlobalBounds().height) - m_position.y : 0));
		
		states.shader = &FadeAnimation::getShaderNonTextured();
		target.draw(m_textBox, states);

		states.transform.translate(TEXT_BOX_X_SPACING, TEXT_BOX_Y_SPACING * 0.8f);
		states.shader = &FadeAnimation::getShaderTextured();
		TextPane::draw(target, states);
	}
}