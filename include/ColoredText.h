#ifndef GUI_COLORED_STRING
#define GUI_COLORED_STRING

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
	typedef std::pair<std::string, sf::Color>(ColoredString);

	class ColoredText final
	{
	public:

		ColoredText(const ColoredText& copy) = default;
		ColoredText(ColoredText&& temp) = default;
		~ColoredText() = default;

		ColoredText& operator=(const ColoredText& copy) = default;
		ColoredText& operator=(ColoredText&& temp) = default;

		const bool isVolatile()const;
		void getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize = 13)const;

		static void arrangeText(unique_ptr_vector<sf::Text>& target);
		
	private:
		
		ColoredText() = default;

		void getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize, const float TEXT_HEIGHT, sf::Vector2f& addPosition)const;

		shared_ptr_vector<ColoredString> text;
		std::unordered_multimap<size_t, std::function<ColoredText()>> volatileText;

		static const float LINE_SPACING;

		friend ColoredText bind(const std::string& string, const sf::Color& color);
		friend ColoredText& operator+(ColoredText& text, const ColoredText& newText);
		friend ColoredText& operator+(ColoredText& text, ColoredText&& newText);
		friend ColoredText& operator+(ColoredText& text, const ColoredString& string);
		friend ColoredText& operator+(ColoredText& text, ColoredString&& string);
		friend ColoredText& operator+(ColoredText& text, const std::function<ColoredText()>& function);
		friend ColoredText& operator+(ColoredText& text, std::function<ColoredText()>&& function);
	};

	ColoredText bind(const std::string& string, const sf::Color& color);
}

#endif