#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>

#include "Interactive.h"

namespace gui 
{
	class TextField final : public Interactive
	{
	public:
		TextField(const std::function<void(const std::string&)>& processTextInputFunction, const unsigned short fieldWidth, const sf::Font& font, const unsigned char characterSize = 13);
		TextField(const TextField& copy);
		TextField(TextField&& temp) = default; 
		TextField() = default;
		~TextField() = default;

		TextField* move()override { return new TextField(std::move(*this)); }

		const bool input(const sf::Event& event)override;
		void processCurrentInput();

		const sf::Vector2f& getPosition()const override;
		const sf::FloatRect getGlobalBounds()const override;
		const std::string& getCurrentInput()const;
		const unsigned char getCharacterSize()const;
		const sf::Color& getTextColor()const;

		TextField& setCharacterSize(const unsigned char characterSize);
		TextField& setTextColor(const sf::Color& color);
		TextField& setPrompt(const std::string& prompt, const sf::Color& color = sf::Color::Black);
		TextField& setPromptColor(const sf::Color& color);
		TextField& clearPrompt();
		TextField& setPosition(const float x, const float y)override;
		TextField& setFont(const sf::Font& font);
		TextField& setFieldWidth(const unsigned short width);
		TextField& setClipboardPermission(const bool allowClipboard);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		mutable sf::View                        box;
		sf::Text                                currentInput;
		std::function<void(const std::string&)> processingFunction;
		std::unique_ptr<sf::Text>               prompt = nullptr;
		bool                                    active = false;
		bool                                    allowClipboard = false;
		unsigned short                          cursorPosition = 0;
		sf::Vector2f                            position = sf::Vector2f(0, 0);
	};
}