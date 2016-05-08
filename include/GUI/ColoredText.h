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

#ifndef GUI_COLORED_STRING
#define GUI_COLORED_STRING

#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

#include <SFML/Graphics/Text.hpp>

namespace gui
{
	template <typename T>
	using unique_ptr_vector = std::vector<std::unique_ptr<T>>;
	template <typename T>
	using shared_ptr_vector = std::vector<std::shared_ptr<T>>;
	typedef std::pair<std::string, std::pair<sf::Color, sf::Text::Style>>(ColoredString);

	class ColoredText final
	{
	public:

		ColoredText(const ColoredText& copy) = default;
		ColoredText(ColoredText&& temp) = default;
		~ColoredText() = default;

		ColoredText& operator=(const ColoredText& copy) = default;
		ColoredText& operator=(ColoredText&& temp) = default;

		operator ColoredString();

		const bool isVolatile()const;
		void getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize = 13)const;

		static void arrangeText(unique_ptr_vector<sf::Text>& target);
		
	private:		
		ColoredText() = default;

		void getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize, const float TEXT_HEIGHT, sf::Vector2f& addPosition)const;

		shared_ptr_vector<ColoredString>                              m_text;
		std::unordered_multimap<size_t, std::function<ColoredText()>> m_volatileText;

		static void stringToArrangedText(unique_ptr_vector<sf::Text>& target,
			const ColoredString& string, const sf::Font& font,
			const unsigned char characterSize, sf::Vector2f& addPosition,
			const float TEXT_HEIGHT);

		static constexpr unsigned char LINE_SPACING = 2;

		friend ColoredText bind(const std::string& string, const sf::Color& color, const sf::Text::Style style);
		friend ColoredText& operator+(ColoredText& text, const ColoredText& newText);
		friend ColoredText& operator+(ColoredText& text, ColoredText&& newText);
		friend ColoredText& operator+(ColoredText& text, const ColoredString& string);
		friend ColoredText& operator+(ColoredText& text, ColoredString&& string);
		friend ColoredText& operator+(ColoredText& text, const std::function<ColoredText()>& function);
		friend ColoredText& operator+(ColoredText& text, std::function<ColoredText()>&& function);
	};

	ColoredText bind(const std::string& string, const sf::Color& color = sf::Color::White, const sf::Text::Style style = sf::Text::Regular);
}

#endif