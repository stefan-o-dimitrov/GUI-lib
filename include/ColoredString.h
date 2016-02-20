#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace gui
{
	template <typename T>
	using unique_ptr_vector = std::vector<std::unique_ptr<T>>;
	template <typename T>
	using shared_ptr_vector = std::vector<std::shared_ptr<T>>;
	typedef std::pair<std::string, sf::Color>(cString);

	class ColoredString final
	{
	public:

		ColoredString(const ColoredString& copy) = default;
		ColoredString(ColoredString&& temp) = default;
		~ColoredString() = default;

		ColoredString& operator=(const ColoredString& copy) = default;
		ColoredString& operator=(ColoredString&& temp) = default;

		const bool isVolatile()const;
		void getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize = 13)const;

		static void arrangeText(unique_ptr_vector<sf::Text>& target);
		
	private:
		
		ColoredString() = default;

		void getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize, const float TEXT_HEIGHT, sf::Vector2f& addPosition)const;

		shared_ptr_vector<cString> text;
		std::unordered_multimap<size_t, std::function<ColoredString()>> volatileText;

		static const float LINE_SPACING;

		friend ColoredString bind(const std::string&, const sf::Color&);
		friend ColoredString& operator+(ColoredString&, const ColoredString&);
		friend ColoredString& operator+(ColoredString&, ColoredString&&);
		friend ColoredString& operator+(ColoredString&, const std::function<ColoredString()>&);
		friend ColoredString& operator+(ColoredString&, std::function<ColoredString()>&&);
	};

	ColoredString bind(const std::string& string, const sf::Color& color);
}