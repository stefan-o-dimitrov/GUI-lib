#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Internals.h"
#include "Interactive.h"
#include "ColoredText.h"

namespace gui 
{
	class TextPane : public Interactive
	{
	public:
		TextPane(const ColoredText& text, const sf::Font& font, const unsigned char characterSize = 13);
		TextPane(const TextPane& copy);
		TextPane(TextPane&& temp) = default;
		TextPane() = default;
		~TextPane() = default;

		TextPane& operator=(const TextPane& copy);
		TextPane& operator=(TextPane&& temp) = default;

		TextPane* copy()const override { return new TextPane(*this); }
		TextPane* move()override { return new TextPane(std::move(*this)); }

		const bool input(const sf::Event& event)override;
		const bool contains(const sf::Vector2f& point)const;
		
		const sf::FloatRect getGlobalBounds()const;
		virtual const sf::Vector2f& getPosition()const;
		const sf::Font& getFont()const;
		const unsigned char getCharacterSize()const;

		virtual TextPane& setPosition(const float x, const float y) override;
		TextPane& setText(const ColoredText& text);
		TextPane& setText(ColoredText&& text);
		TextPane& setFont(const sf::Font& font);
		TextPane& setCharacterSize(const unsigned char characterSize);

	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
		virtual void update()const;

		sf::Vector2f position = sf::Vector2f(0, 0);

	private:
		ColoredText                         string;
		const sf::Font*                     font = nullptr;
		unsigned char                       characterSize = 13;
		mutable TimePoint                   timeOfLastUpdate;
		mutable unique_ptr_vector<sf::Text> text;
	};
};
