#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <memory>

#include "ColoredText.h"
#include "Internals.h"
#include "TextPane.h"

namespace gui 
{
	class HoverMessage final : public TextPane
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
		const sf::Color& getBackgroundFill()const;
		const sf::Color& getBorderFill()const;
		const char getBorderThickness()const;

		HoverMessage& setBackgroundFill(const sf::Color& color);
		HoverMessage& setBorderFill(const sf::Color& color);
		HoverMessage& setBorderThickness(const char thickness);

	private:

		const sf::Vector2f& getPosition()const override;
		HoverMessage& setPosition(const float x, const float y)override;
		HoverMessage& setPosition(const sf::Vector2f& position)override;
		HoverMessage& setFadeDirection(const bool direction);

		void updateFadeAmount()const;
		void updateBox()const;
		void update()const override;

		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		mutable TimePoint timeOfLastAnimationStep;
		mutable float fadeAmount = 0.0f;
		mutable bool fadeDirection = false;
		mutable sf::RectangleShape textBox;

		static const bool loadShader();

		static sf::Shader fadeShaderTextured, fadeShader;
		static const std::string FADE_SHADER_TEXTURED, FADE_SHADER;
		static const bool shaderLoadSuccessful;
		static const float FADE_ANIMATION_DURATION;
		static const unsigned char TEXT_BOX_X_SPACING, TEXT_BOX_Y_SPACING, FADE_ANIMATION_FPS;
	};
};