#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace gui
{
	typedef std::pair<std::string, sf::Color>(cString);
	class ColoredString final
	{
	public:
		ColoredString(const ColoredString&);
		ColoredString(ColoredString&&) = default;

		ColoredString& operator=(const ColoredString&);
		ColoredString& operator=(ColoredString&&) = default;
	private:
		friend class Button;
		friend class TextArea;
		friend class TextPane;
		friend class HoverMessage;

		friend ColoredString bind(const std::string&, const sf::Color&);
		friend ColoredString operator+(ColoredString&, ColoredString&&);

		ColoredString() = default;
		
		std::vector<std::unique_ptr<cString>> vec;

		static std::vector<std::unique_ptr<sf::Text>> interpret(const ColoredString& string, const sf::Font& font, const unsigned char characterSize = 13);

		static const float LINE_SPACING;
	};

	ColoredString bind(const std::string& string, const sf::Color& color);
}