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

#ifndef GUI_TEXT_FIELD
#define GUI_TEXT_FIELD

#include <functional>
#include <memory>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Interactive.h"
#include "ColoredText.h"
#include "Internals.h"

namespace gui 
{
	class TextField final : public Interactive
	{
	public:
		TextField(const sf::Font& font, const unsigned short fieldWidth, const unsigned char characterSize = 13);
		TextField(const TextField& copy);
		TextField(TextField&& temp) = default; 
		TextField();
		~TextField() = default;

		std::unique_ptr<Interactive> copy()const override;
		std::unique_ptr<Interactive> move()override;

		const bool input(const sf::Event& event)override;
		void lostFocus()override;
		void setInactive();
		void clear();
		void processCurrentInput();
		void setCursorPosition(size_t position);

		const sf::FloatRect getGlobalBounds()const override;
		const sf::Vector2f& getPosition()const override;

		const sf::String& getCurrentInput()const;
		const unsigned char getCharacterSize()const;
		const sf::Color& getTextColor()const;
		size_t getCursorPosition()const;
		
		TextField& setPosition(const float x, const float y)override;
		TextField& setPosition(const sf::Vector2f& position)override;

		TextField& setInputProcessingFunction(const std::function<void(const sf::String&)>& function);
		TextField& setInputProcessingFunction(std::function<void(const sf::String&)>&& function);
		TextField& setCharacterSize(const unsigned char characterSize);
		TextField& setColor(const sf::Color& color);
		TextField& setCursorColor(const sf::Color& color);
		TextField& setPrompt(const ColoredString& prompt);
		TextField& setPromptColor(const sf::Color& color);
		TextField& setPromptStyle(const sf::Text::Style style);
		TextField& clearPrompt();
		TextField& setFont(const sf::Font& font);
		TextField& setWidth(const unsigned short width);
		TextField& clearAfterInputIsProcessed(const bool shoudlClear);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void removeCharacter(const bool backspace = true);
		void addCharacter(const sf::Uint32 character);
		void getClickedCharacter(const float x, const float y);

		sf::Vector2f                           m_position = sf::Vector2f(0, 0);
		sf::Text                               m_input, m_cursor;
		std::function<void(const sf::String&)> m_processingFunction;
		bool                                   m_active = false, m_clearAfterInputProcessed = true;
		unsigned short                         m_cursorPosition = 0;
		std::unique_ptr<sf::Text>              m_prompt = nullptr;
		mutable sf::View                       m_box;
		mutable TimePoint                      m_timeOfLastCursorFlick;
		mutable bool                           m_cursorVisible = true;
	};
}

#endif