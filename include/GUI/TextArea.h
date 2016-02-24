#ifndef GUI_TEXT_AREA
#define GUI_TEXT_AREA

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <memory>
#include <functional>

#include "Internals.h"
#include "Interactive.h"
#include "HoverMessage.h"
#include "Hoverable.h"

namespace gui 
{
	class TextArea final : public Hoverable
	{
		friend class Button;
	public:
		TextArea(const std::string& text, const sf::Font& font, const unsigned char characterSize = 13);
		TextArea(const TextArea& copy);
		TextArea(TextArea&& temp) = default;
		TextArea() = default;
		~TextArea() = default;

		std::unique_ptr<Interactive> copy()const override;
		std::unique_ptr<Interactive> move()override;

		using Hoverable::input;
		const bool contains(const sf::Vector2f& point)const override;

		const sf::FloatRect getGlobalBounds()const override;
		const sf::Vector2f& getPosition()const override;

		const sf::Font& getFont()const;
		const unsigned char getCharacterSize()const;

		TextArea& clearMessage()override;
		TextArea& setMessage(const HoverMessage& message)override;
		TextArea& setMessage(HoverMessage&& message)override;
		TextArea& setPosition(const float x, const float y)override;
		TextArea& setPosition(const sf::Vector2f& position)override;

		TextArea& setText(const ColoredString& text)const;
		TextArea& setFont(const sf::Font& font);
		TextArea& setCharacterSize(const unsigned char characterSize);
		TextArea& setColor(const sf::Color& color);
		TextArea& setUpdateFunction(const std::function<ColoredString()>& func);
		TextArea& setUpdateFunction(std::function<ColoredString()>&& func);
		
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		mutable sf::Text text;
		mutable TimePoint timeOfLastUpdate;
		std::unique_ptr<std::function<ColoredString()>> updateFunction = nullptr;
	};
}

#endif
