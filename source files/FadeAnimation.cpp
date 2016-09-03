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

#include "../include/GUI/FadeAnimation.h"

namespace gui
{
	const std::string FADE_SHADER_TEXTURED =
		"uniform sampler2D tex;\
		uniform float amount;\
		\
		void main()\
		{\
			vec4 color = texture2D( tex, gl_TexCoord[0].xy ) * gl_Color;\
			gl_FragColor = vec4(color.r, color.g, color.b, color.a * amount);\
		}",
		FADE_SHADER_NO_TEXTURE =
		"uniform float amount;\
		\
		void main()\
		{\
			gl_FragColor = vec4(gl_Color.r, gl_Color.g, gl_Color.b, gl_Color.a * amount);\
		}\
		";

	sf::Shader FadeAnimation::fadeShaderTextured, FadeAnimation::fadeShaderNoTexture;
	const bool FadeAnimation::shaderLoadSuccessful = FadeAnimation::loadShader();

	FadeAnimation& FadeAnimation::setDuration(const float duration)
	{
		Animation::setDuration(duration);
		return *this;
	}

	FadeAnimation& FadeAnimation::setFPS(const float fps)
	{
		Animation::setFPS(fps);
		return *this;
	}

	FadeAnimation& FadeAnimation::setFadeDirection(const bool direction)
	{
		m_fadeDirection = direction;
		return *this;
	}

	void FadeAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (((m_fadeAmount < 1.0f && m_fadeDirection) || (m_fadeAmount > 0.0f && !m_fadeDirection)))
			Animation::draw(target, states);

		if (shaderLoadSuccessful)
		{
			fadeShaderNoTexture.setUniform("amount", m_fadeAmount);
			fadeShaderTextured.setUniform("amount", m_fadeAmount);
		}
	}

	void FadeAnimation::step() const
	{
		m_fadeAmount += (m_fadeDirection ? 1 : -1) / (getAnimationFPS() * getAnimationDuration());
		if (m_fadeAmount > 1.0f) m_fadeAmount = 1.0f;
		else if (m_fadeAmount < 0.0f) m_fadeAmount = 0.0f;
	}

	const bool FadeAnimation::getFadeDirection() const
	{
		return m_fadeDirection;
	}

	const float FadeAnimation::getFadeAmount() const
	{
		return m_fadeAmount;
	}
	
	const sf::Shader& FadeAnimation::getShaderNonTextured() const
	{
		return fadeShaderNoTexture;
	}

	const sf::Shader& FadeAnimation::getShaderTextured() const
	{
		return fadeShaderTextured;
	}
	
	const bool FadeAnimation::loadShader()
	{
		if (fadeShaderNoTexture.loadFromMemory(FADE_SHADER_NO_TEXTURE, sf::Shader::Fragment) &&
			fadeShaderTextured.loadFromMemory(FADE_SHADER_TEXTURED, sf::Shader::Fragment))
		{
			fadeShaderTextured.setUniform("tex", sf::Shader::CurrentTexture);
			return true;
		}
		return true;
	}
}