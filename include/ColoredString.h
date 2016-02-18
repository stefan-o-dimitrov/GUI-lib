#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace gui
{
	template <typename T>
	using ptr_vector = std::vector<std::unique_ptr<T>>;
	typedef std::pair<std::string, sf::Color>(cString);
	class ColoredString final
	{
	public:

		ColoredString(const ColoredString&);
		ColoredString(ColoredString&&) = default;
		~ColoredString() = default;

		ColoredString& operator=(const ColoredString&);
		ColoredString& operator=(ColoredString&&) = default;
		
	private:

		friend class Button;
		friend class TextArea;
		friend class TextPane;
		friend class HoverMessage;

		ColoredString() = default;

		ptr_vector<cString> text;
		std::unordered_multimap<size_t, std::function<ColoredString()>> volatileText;

		static ptr_vector<sf::Text> interpret(const ColoredString& string, const sf::Font& font, const unsigned char characterSize = 13);
		static ptr_vector<sf::Text> rearrangeText(const ptr_vector<sf::Text>& string, const sf::Font& font, const unsigned char characterSize = 13);

		static const float LINE_SPACING;

		friend ColoredString bind(const std::string&, const sf::Color&);
		friend ColoredString& operator+(ColoredString&, ColoredString&&);
		friend ColoredString& operator+(ColoredString&, const std::function<ColoredString()>&);
		friend ColoredString& operator+(ColoredString&, std::function<ColoredString()>&&);
	};

	ColoredString bind(const std::string& string, const sf::Color& color);
}