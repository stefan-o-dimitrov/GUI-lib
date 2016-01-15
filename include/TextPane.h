#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Internals.h"
#include "Interactive.h"
#include "Hoverable.h"
#include "ColoredString.h"

namespace gui 
{
	class TextPane final : public Hoverable
	{
	public:
		TextPane(const ColoredString& text, const sf::Font& font, const unsigned char characterSize = 13);
		TextPane(const TextPane& copy);
		TextPane(TextPane&& temp) = default;
		TextPane() = default;
		~TextPane() = default;

		using Hoverable::input;
		const bool contains(const sf::Vector2f& point)const override;

		TextPane& operator=(const TextPane& copy);
		TextPane& operator=(TextPane&& temp) = default;

		const sf::FloatRect getGlobalBounds()const override;
		const sf::Vector2f& getPosition()const override;
		const sf::Font& getFont()const;
		const unsigned char getCharacterSize()const;

		TextPane& setPosition(const float x, const float y) override;
		TextPane& setText(const ColoredString& text);
		TextPane& setFont(const sf::Font& font);
		TextPane& setCharacterSize(const unsigned char characterSize);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		sf::Vector2f                           position = sf::Vector2f(0, 0);
		const sf::Font*                        font = nullptr;
		unsigned char                          characterSize = 13;
		std::vector<std::unique_ptr<sf::Text>> text;
	};
};
