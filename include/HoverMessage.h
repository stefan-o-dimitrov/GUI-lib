#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <memory>

#include "ColoredText.h"
#include "Internals.h"

namespace gui 
{
	class HoverMessage final : public sf::Drawable
	{
		friend class Hoverable;
	public:
		HoverMessage(const ColoredText& string, const sf::Font& font, const unsigned char characterSize = 13);
		HoverMessage(ColoredText&& string, const sf::Font& font, const unsigned char characterSize = 13);
		HoverMessage(const HoverMessage& copy);
		HoverMessage(HoverMessage&& temp);

		HoverMessage& operator=(const HoverMessage& copy);
		HoverMessage& operator=(HoverMessage&& temp);
		
		const sf::FloatRect getGlobalBounds()const;
		const sf::Font& getFont()const;
		const unsigned char getCharacterSize()const;
		const sf::Color& getBackgroundFill()const;
		const sf::Color& getBorderFill()const;
		const char getBorderThickness()const;

		HoverMessage& setText(const ColoredText& text);
		HoverMessage& setText(ColoredText&& text);
		HoverMessage& setFont(const sf::Font& font);
		HoverMessage& setCharacterSize(const unsigned char characterSize);
		HoverMessage& setBackgroundFill(const sf::Color& color);
		HoverMessage& setBorderFill(const sf::Color& color);
		HoverMessage& setBorderThickness(const char thickness);

	private:

		void updateBox()const;
		const sf::Vector2f& getPosition()const;
		HoverMessage& setPosition(const float x, const float y);
		HoverMessage& setPosition(const sf::Vector2f& position);

		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		sf::Vector2f                        position = sf::Vector2f(0, 0);
		const sf::Font*                     font = nullptr;
		unsigned char                       characterSize = 13;
		ColoredText                         string;
		mutable unique_ptr_vector<sf::Text> text;
		mutable sf::RectangleShape          textBox;
		mutable TimePoint                   timeOfLastUpdate;

		static const unsigned char TEXT_UPS, TEXT_BOX_X_SPACING, TEXT_BOX_Y_SPACING;
	};
};