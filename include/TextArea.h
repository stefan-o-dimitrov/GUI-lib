#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>

#include "Internals.h"
#include "Interactive.h"
#include "Hoverable.h"

namespace gui {
	class TextArea final : public Hoverable
	{
		friend class Button;
	public:
		TextArea(const std::string& text, const sf::Font& font, const unsigned char characterSize = 13);
		TextArea(const TextArea& copy);
		TextArea(TextArea&& temp) = default;
		TextArea() = default;
		~TextArea() = default;

		TextArea* copy()const override { return new TextArea(*this); }
		TextArea* move()override { return new TextArea(std::move(*this)); }

		using Hoverable::input;
		const bool contains(const sf::Vector2f& point)const override;

		const sf::FloatRect getGlobalBounds()const override;
		const sf::Vector2f& getPosition()const override;
		const sf::Font& getFont()const;
		const unsigned char getCharacterSize()const;

		TextArea& setPosition(const float x, const float y)override;
		TextArea& setText(const ColoredString& text)const;
		TextArea& setFont(const sf::Font& font);
		TextArea& setCharacterSize(const unsigned char characterSize);
		TextArea& setColor(const sf::Color& color) { text.setColor(color); return *this; }
		TextArea& setUpdateFunction(const std::function<ColoredString()>& func) { updateFunction.reset(new std::function<ColoredString()>(func)); return *this; };
		TextArea& setUpdateFunction(std::function<ColoredString()>&& func) { updateFunction.reset(new std::function<ColoredString()>(std::move(func))); return *this; };
		
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		mutable sf::Text text;
		std::unique_ptr<std::function<ColoredString()>> updateFunction = nullptr;
		mutable TimePoint timeOfLastUpdate;

		static const unsigned char TEXT_UPS;
	};
};
