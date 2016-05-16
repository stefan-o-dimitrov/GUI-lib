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

#ifndef GUI_HOVER_MESSAGE
#define GUI_HOVER_MESSAGE

#include <unordered_map>
#include <vector>
#include <memory>

#include <SFML/Graphics/RectangleShape.hpp>

#include "ColoredText.h"
#include "TextPane.h"
#include "FadeAnimation.h"

namespace gui 
{
	class HoverMessage final : private TextPane, public FadeAnimation
	{
	public:
		HoverMessage(const ColoredText& string, const sf::Font& font, const unsigned char characterSize = 13);
		HoverMessage(ColoredText&& string, const sf::Font& font, const unsigned char characterSize = 13);
		HoverMessage(const HoverMessage& copy);
		HoverMessage(HoverMessage&& temp);

		HoverMessage& operator=(const HoverMessage& copy);
		HoverMessage& operator=(HoverMessage&& temp);

		const sf::Vector2f& getPosition()const override;
		const sf::Font& getFont()const override;
		const unsigned char getCharacterSize()const override;

		const sf::FloatRect getGlobalBounds()const;
		const sf::Color& getBackgroundFill()const;
		const sf::Color& getBorderFill()const;
		const char getBorderThickness()const;
		const ColoredText& getText()const;
		ColoredText& getText();
		
		HoverMessage& setPosition(const float x, const float y)override;
		HoverMessage& setPosition(const sf::Vector2f& position)override;
		HoverMessage& setText(const ColoredText& text)override;
		HoverMessage& setText(ColoredText&& text)override;
		HoverMessage& setFont(const sf::Font& font)override;
		HoverMessage& setCharacterSize(const unsigned char characterSize)override;
		HoverMessage& setFadeDirection(const bool direction)override;
		HoverMessage& setDuration(const float duration)override;

		HoverMessage& setBackgroundFill(const sf::Color& color);
		HoverMessage& setBorderFill(const sf::Color& color);
		HoverMessage& setBorderThickness(const char thickness);

	private:
		void update()const override;
		void updateBox()const;

		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		mutable sf::RectangleShape m_textBox;

		static const unsigned char TEXT_BOX_X_SPACING, TEXT_BOX_Y_SPACING;
	};
};

#endif