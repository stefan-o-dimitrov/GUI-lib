#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Internals.h"
#include "Interactive.h"
#include "Hoverable.h"
#include "ColoredText.h"

namespace gui 
{
	class TextPane final : public Hoverable
	{
	public:
		TextPane(const ColoredText& text, const sf::Font& font, const unsigned char characterSize = 13);
		TextPane(const TextPane& copy);
		TextPane(TextPane&& temp) = default;
		TextPane() = default;
		~TextPane() = default;

		TextPane* copy()const override { return new TextPane(*this); }
		TextPane* move()override { return new TextPane(std::move(*this)); }

		using Hoverable::input;
		const bool contains(const sf::Vector2f& point)const override;
		
		const sf::FloatRect getGlobalBounds()const override;
		const sf::Vector2f& getPosition()const override;
		const sf::Font& getFont()const;
		const unsigned char getCharacterSize()const;

		TextPane& setPosition(const float x, const float y) override;
		TextPane& setText(const ColoredText& text);
		TextPane& setText(ColoredText&& text);
		TextPane& setFont(const sf::Font& font);
		TextPane& setCharacterSize(const unsigned char characterSize);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		ColoredText                         string;
		sf::Vector2f                        position = sf::Vector2f(0, 0);
		const sf::Font*                     font = nullptr;
		unsigned char                       characterSize = 13;
		mutable TimePoint                   timeOfLastUpdate;
		mutable unique_ptr_vector<sf::Text> text;

		static const unsigned char TEXT_UPS;
	};
};
