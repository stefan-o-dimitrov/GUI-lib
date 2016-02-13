#pragma once

#include <SFML/Graphics.hpp>

#include "Button.h"

namespace gui
{
	class CheckBox final : public Button
	{
	public:
		CheckBox(const Button& falseState, const Button& trueState, const sf::Vector2f& position = sf::Vector2f(0, 0), const bool state = false);
		CheckBox(Button&& falseState, Button&& trueState, const sf::Vector2f& position = sf::Vector2f(0, 0), const bool state = false);
		CheckBox(const CheckBox& copy) = default;
		CheckBox(CheckBox&& temp) = default;
		CheckBox() = default;
		~CheckBox() = default;

		CheckBox* copy()const override { return new CheckBox(*this); }
		CheckBox* move()override { return new CheckBox(std::move(*this)); }

		const bool input(const sf::Event& event)override;
		const bool contains(const sf::Vector2f& point)const;

		const State getState()const override;
		const bool isChecked()const;

		const sf::FloatRect getTrueStateGlobalBounds()const;
		const sf::Texture& getTrueStateTexture()const;
		const bool getTrueStateTransparencyCheck()const;
		const sf::IntRect& getTrueStateTextureRect()const;
		const std::shared_ptr<const HoverMessage> getTrueStateMessage()const;
		const std::shared_ptr<const HoverMessage> getTrueStatePredicateMessage()const;
		
		void toggle();

		CheckBox& setIsChecked(const bool isChecked);
		CheckBox& setPosition(const float x, const float y)override;
		using Interactive::setPosition;

		CheckBox& setTrueStateTexture(const sf::Texture& texture, const bool transparencyCheck = false);
		CheckBox& setTrueStateTransparencyCheck(const bool transparencyCheck);
		CheckBox& setTrueStateTextureRect(const sf::IntRect& textureRect);
		CheckBox& setTrueStateClickSound(const unsigned short soundKey);
		CheckBox& setTrueStateMessage(const HoverMessage& message);
		CheckBox& setTrueStateMessage(HoverMessage&& messageTemp);
		CheckBox& setTrueStatePredicates(const predicateArray& predicates, const sf::Font& font, const unsigned char characterSize = 13);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
		
		using Button::getName;
		using Button::setName;

		Button trueState;
		bool   checked = false;
	};
};