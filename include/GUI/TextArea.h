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

#ifndef GUI_TEXT_AREA
#define GUI_TEXT_AREA

#include <memory>
#include <functional>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Internals.h"
#include "Interactive.h"
#include "HoverMessage.h"
#include "Hoverable.h"

namespace gui 
{
	class TextArea final : public Hoverable
	{
		friend class Button;
	public:
		TextArea(const sf::String& text, const sf::Font& font, const unsigned char characterSize = 13);
		TextArea(const TextArea& copy);
		TextArea(TextArea&& temp) = default;
		TextArea() = default;
		~TextArea() = default;

		std::unique_ptr<Interactive> copy()const override;
		std::unique_ptr<Interactive> move()override;

		using Hoverable::lostFocus;
		using Hoverable::input;
		const bool contains(const float x, const float y)const override;
		using Interactive::contains;

		const sf::FloatRect getGlobalBounds()const override;
		const sf::Vector2f& getPosition()const override;

		const sf::Font& getFont()const;
		const unsigned char getCharacterSize()const;
		const sf::Color& getColor()const;
		const sf::String& getText()const;
		const sf::Text::Style getStyle()const;

		TextArea& clearMessage()override;
		TextArea& setMessage(const HoverMessage& message)override;
		TextArea& setMessage(HoverMessage&& message)override;
		TextArea& setPosition(const float x, const float y)override;
		TextArea& setPosition(const sf::Vector2f& position)override;

		TextArea& setStyle(const sf::Text::Style style)const;
		TextArea& setText(const sf::String& text)const;
		TextArea& setFont(const sf::Font& font);
		TextArea& setCharacterSize(const unsigned char characterSize);
		TextArea& setColor(const sf::Color& color)const;
		TextArea& setUpdateFunction(const std::function<ColoredString()>& func);
		TextArea& setUpdateFunction(std::function<ColoredString()>&& func);
		
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		mutable sf::Text                                      m_text;
		mutable TimePoint                                     m_timeOfLastUpdate;
		std::shared_ptr<const std::function<ColoredString()>> m_updateFunction;
	};
}

#endif
