#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>

namespace gui
{
	typedef std::pair<std::string, sf::Color>(cString);
	class ColoredString final
	{
	public:

		ColoredString(const ColoredString&);
		ColoredString(ColoredString&&) = default;
		~ColoredString() = default;
		
	private:

		friend class Button;
		friend class TextArea;
		friend class TextPane;
		friend class HoverMessage;

		ColoredString() = default;
		
		std::vector<std::unique_ptr<cString>> text;
		std::vector<std::pair<std::function<ColoredString()>, size_t>> volatileText;

		static std::vector<std::unique_ptr<sf::Text>> interpret(const ColoredString& string, const sf::Font& font, const unsigned char characterSize = 13);
		static std::vector<std::unique_ptr<sf::Text>> reinterpret(const std::vector<std::unique_ptr<sf::Text>>& string, const sf::Font& font, const unsigned char characterSize = 13);

		static const float LINE_SPACING;

		friend ColoredString bind(const std::string&, const sf::Color&);
		friend ColoredString& operator+(ColoredString&, ColoredString&&);
		friend ColoredString& operator+(ColoredString&, const std::function<ColoredString()>&);
		friend ColoredString& operator+(ColoredString&, std::function<ColoredString()>&&);
	};

	ColoredString bind(const std::string& string, const sf::Color& color);
}