////////////////////////////////////////////////////////////
//
// GUI-lib - Graphical User Interface Library
// Copyright (C) 2014-2016 ShtGames (shtgamessts@gmail.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef GUI_HOVER_MESSAGE
#define GUI_HOVER_MESSAGE

#include <unordered_map>
#include <vector>
#include <memory>

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "ColoredText.h"
#include "Internals.h"
#include "TextPane.h"

namespace gui 
{
	class HoverMessage final : public TextPane
	{
	public:
		HoverMessage(const ColoredText& string, const sf::Font& font, const unsigned char characterSize = 13);
		HoverMessage(ColoredText&& string, const sf::Font& font, const unsigned char characterSize = 13);
		HoverMessage(const HoverMessage& copy);
		HoverMessage(HoverMessage&& temp);

		HoverMessage& operator=(const HoverMessage& copy);
		HoverMessage& operator=(HoverMessage&& temp);

		const sf::Vector2f& getPosition()const override;
		const sf::FloatRect getGlobalBounds()const;
		const sf::Color& getBackgroundFill()const;
		const sf::Color& getBorderFill()const;
		const char getBorderThickness()const;
		const float getFadeAmount()const;

		HoverMessage& setBackgroundFill(const sf::Color& color);
		HoverMessage& setBorderFill(const sf::Color& color);
		HoverMessage& setBorderThickness(const char thickness);
		HoverMessage& setPosition(const float x, const float y)override;
		HoverMessage& setPosition(const sf::Vector2f& position)override;
		HoverMessage& setFadeDirection(const bool direction);


	private:
		void updateFadeAmount()const;
		void updateBox()const;
		void update()const override;

		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		mutable TimePoint timeOfLastAnimationStep;
		mutable float fadeAmount = 0.0f;
		mutable bool fadeDirection = false;
		mutable sf::RectangleShape textBox;

		static const bool loadShader();

		static sf::Shader fadeShaderTextured, fadeShaderNoTexture;
		static const bool shaderLoadSuccessful;
		static const float FADE_ANIMATION_DURATION;
		static const unsigned char TEXT_BOX_X_SPACING, TEXT_BOX_Y_SPACING, FADE_ANIMATION_FPS;
	};
};

#endif