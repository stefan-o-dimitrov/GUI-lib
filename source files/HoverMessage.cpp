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

#include "../include/GUI/HoverMessage.h"
#include "../include/GUI/Internals.h"

namespace gui
{
	sf::Shader HoverMessage::fadeShaderTextured, HoverMessage::fadeShaderNoTexture;
	const std::string FADE_SHADER_TEXTURED =
		"uniform sampler2D tex;\
		uniform float fadeAmount;\
		\
		void main()\
		{\
			vec4 color = texture2D( tex, gl_TexCoord[0].xy ) * gl_Color;\
			gl_FragColor = vec4(color.r, color.g, color.b, color.a * fadeAmount);\
		}",
		FADE_SHADER_NO_TEXTURE =
		"uniform float fadeAmount;\
		\
		void main()\
		{\
			gl_FragColor = vec4(gl_Color.r, gl_Color.g, gl_Color.b, gl_Color.a * fadeAmount);\
		}\
		";
	const bool HoverMessage::shaderLoadSuccessful = HoverMessage::loadShader();
	const float HoverMessage::FADE_ANIMATION_DURATION = 0.25f;
	const unsigned char HoverMessage::TEXT_BOX_X_SPACING = 10, HoverMessage::TEXT_BOX_Y_SPACING = 10,
		HoverMessage::FADE_ANIMATION_FPS = 20;

	HoverMessage::HoverMessage(const ColoredText& str, const sf::Font& font, const unsigned char characterSize)
		: TextPane(str, font, characterSize)
	{
		textBox.setFillColor(sf::Color(0, 0, 0, 255));
		update();
	}

	HoverMessage::HoverMessage(ColoredText&& str, const sf::Font& font, const unsigned char characterSize)
		: TextPane(std::move(str), font, characterSize)
	{
		textBox.setFillColor(sf::Color(0, 0, 0, 255));
		update();
	}

	HoverMessage::HoverMessage(const HoverMessage& copy)
		: TextPane(copy), textBox(copy.textBox)	{}

	HoverMessage::HoverMessage(HoverMessage&& temp)
		: TextPane(std::move(temp)), textBox(temp.textBox) {}

	HoverMessage& HoverMessage::operator=(const HoverMessage& copy)
	{
		TextPane::operator=(copy);
		textBox = copy.textBox;
		return *this;
	}

	HoverMessage& HoverMessage::operator=(HoverMessage&& temp)
	{
		TextPane::operator=(std::move(temp));
		textBox = temp.textBox;
		return *this;
	}

	const sf::FloatRect HoverMessage::getGlobalBounds()const
	{
		return textBox.getGlobalBounds();
	}

	const sf::Color& HoverMessage::getBackgroundFill()const
	{
		return textBox.getFillColor();
	}

	const sf::Color& HoverMessage::getBorderFill()const
	{
		return textBox.getOutlineColor();
	}

	const char HoverMessage::getBorderThickness()const
	{
		return textBox.getOutlineThickness();
	}

	const float HoverMessage::getFadeAmount() const
	{
		return fadeAmount;
	}

	void HoverMessage::updateFadeAmount() const
	{
		if ((fadeAmount < 1.0f && fadeDirection) || (fadeAmount > 0.0f && !fadeDirection) &&
			Duration(Internals::timeSinceStart() - timeOfLastAnimationStep).count() > 1.0f / FADE_ANIMATION_FPS)
		{
			fadeAmount += (fadeDirection ? 1.0f : -1.0f) / (FADE_ANIMATION_FPS * FADE_ANIMATION_DURATION);
			timeOfLastAnimationStep = Internals::timeSinceStart();
			if (fadeAmount > 1.0f) fadeAmount = 1.0f;
			else if (fadeAmount < 0.0f)	fadeAmount = 0.0f;
		}
	}

	void HoverMessage::updateBox()const
	{
		const sf::FloatRect rect = TextPane::getGlobalBounds();
		textBox.setSize(sf::Vector2f(rect.width + TEXT_BOX_X_SPACING * 2, rect.height + TEXT_BOX_Y_SPACING * 2));
	}

	void HoverMessage::update() const
	{
		TextPane::update();
		updateBox();
	}

	const sf::Vector2f& HoverMessage::getPosition()const
	{
		return position;
	}

	HoverMessage& HoverMessage::setPosition(const float x, const float y)
	{
		position.x = x;
		position.y = y;
		textBox.setPosition(position);
		return *this;
	}

	HoverMessage& HoverMessage::setPosition(const sf::Vector2f& pos)
	{
		return setPosition(pos.x, pos.y);
	}

	HoverMessage& HoverMessage::setFadeDirection(const bool direction)
	{
		fadeDirection = direction;
		return *this;
	}
	
	HoverMessage& HoverMessage::setBackgroundFill(const sf::Color& color)
	{
		textBox.setFillColor(color);
		return *this;
	}

	HoverMessage& HoverMessage::setBorderFill(const sf::Color& color)
	{
		textBox.setOutlineColor(color);
		return *this;
	}

	HoverMessage& HoverMessage::setBorderThickness(const char thickness)
	{
		textBox.setOutlineThickness(thickness);
		return *this;
	}
	
	void HoverMessage::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		updateFadeAmount();
		if (fadeAmount == 0.0f) return;

		if (shaderLoadSuccessful)
		{
			fadeShaderNoTexture.setParameter("fadeAmount", fadeAmount);
			fadeShaderTextured.setParameter("fadeAmount", fadeAmount);
		}

		sf::Vector2f pos = sf::Vector2f(position.x + textBox.getGlobalBounds().width > target.getSize().x ?
			(target.getSize().x - textBox.getGlobalBounds().width) - position.x : 0,
			position.y + textBox.getGlobalBounds().height > target.getSize().y ?
			(target.getSize().y - textBox.getGlobalBounds().height) - position.y : 0);

		states.transform.translate(pos);
		
		states.shader = shaderLoadSuccessful ? &fadeShaderNoTexture : nullptr;
		target.draw(textBox, states);

		states.transform.translate(TEXT_BOX_X_SPACING, TEXT_BOX_Y_SPACING);
		states.shader = shaderLoadSuccessful ? &fadeShaderTextured : nullptr;
		TextPane::draw(target, states);
	}

	const bool HoverMessage::loadShader()
	{
		if (fadeShaderNoTexture.loadFromMemory(FADE_SHADER_NO_TEXTURE, sf::Shader::Fragment)
			&& fadeShaderTextured.loadFromMemory(FADE_SHADER_TEXTURED, sf::Shader::Fragment))
		{
			fadeShaderTextured.setParameter("tex", sf::Shader::CurrentTexture);
			return true;
		}
		else return false;
	}
}