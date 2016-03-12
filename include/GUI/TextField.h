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

namespace gui 
{
	class TextField final : public Interactive
	{
	public:
		TextField(const std::function<void(const std::string&)>& inputProcessFunction, const unsigned short fieldWidth, 
			const sf::Font& font, const unsigned char characterSize = 13);
		TextField(const TextField& copy);
		TextField(TextField&& temp) = default; 
		TextField() = default;
		~TextField() = default;

		std::unique_ptr<Interactive> copy()const override;
		std::unique_ptr<Interactive> move()override;

		const bool input(const sf::Event& event)override;
		void processCurrentInput();

		const sf::FloatRect getGlobalBounds()const override;
		const sf::Vector2f& getPosition()const override;

		const std::string& getCurrentInput()const;
		const unsigned char getCharacterSize()const;
		const sf::Color& getTextColor()const;
		
		TextField& setPosition(const float x, const float y)override;
		TextField& setPosition(const sf::Vector2f& position)override;

		TextField& setCharacterSize(const unsigned char characterSize);
		TextField& setTextColor(const sf::Color& color);
		TextField& setPrompt(const std::string& prompt, const sf::Color& color = sf::Color::Black);
		TextField& setPromptColor(const sf::Color& color);
		TextField& clearPrompt();
		TextField& setFont(const sf::Font& font);
		TextField& setFieldWidth(const unsigned short width);
		TextField& setClipboardPermission(const bool allowClipboard);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		sf::Vector2f position = sf::Vector2f(0, 0);
		sf::Text currentInput;
		std::function<void(const std::string&)> processingFunction;
		bool active = false, allowClipboard = false;
		unsigned short cursorPosition = 0;
		std::unique_ptr<sf::Text> prompt = nullptr;
		mutable sf::View box;
	};
}

#endif