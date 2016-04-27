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
	const float FadeAnimation::ANIMATION_FPS = 20;

	FadeAnimation& FadeAnimation::setFadeDirection(const bool direction)
	{
		m_fadeDirection = direction;
		return *this;
	}

	FadeAnimation& FadeAnimation::setAnimationDuration(const float duration)
	{
		m_animationDuration = duration;
		return *this;
	}

	FadeAnimation& FadeAnimation::updateFadeAmount() const
	{
		if ((m_fadeAmount < 1.0f && m_fadeDirection) || (m_fadeAmount > 0.0f && !m_fadeDirection) &&
			Duration(Internals::timeSinceStart() - m_timeOfLastAnimationStep).count() > 1.0f / ANIMATION_FPS)
		{
			m_fadeAmount += (m_fadeDirection ? 1.0f : -1.0f) / (ANIMATION_FPS * m_animationDuration);
			m_timeOfLastAnimationStep = Internals::timeSinceStart();
			if (m_fadeAmount > 1.0f) m_fadeAmount = 1.0f;
			else if (m_fadeAmount < 0.0f) m_fadeAmount = 0.0f;
		}
		return (FadeAnimation&)*this;
	}

	const bool FadeAnimation::getFadeDirection() const
	{
		return m_fadeDirection;
	}

	const float FadeAnimation::getFadeAmount() const
	{
		return m_fadeAmount;
	}

	const float FadeAnimation::getAnimationDuration() const
	{
		return m_animationDuration;
	}

	const sf::Shader& FadeAnimation::getShaderNonTextured() const
	{
		return fadeShaderNoTexture;
	}

	const sf::Shader& FadeAnimation::getShaderTextured() const
	{
		return fadeShaderTextured;
	}

	void FadeAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		updateFadeAmount();
		if (m_fadeAmount == 0.0f) return;

		if (shaderLoadSuccessful)
		{
			fadeShaderNoTexture.setParameter("amount", m_fadeAmount);
			fadeShaderTextured.setParameter("amount", m_fadeAmount);
		}
	}

	const bool FadeAnimation::loadShader()
	{
		if (fadeShaderNoTexture.loadFromMemory(FADE_SHADER_NO_TEXTURE, sf::Shader::Fragment) &&
			fadeShaderTextured.loadFromMemory(FADE_SHADER_TEXTURED, sf::Shader::Fragment))
		{
			fadeShaderTextured.setParameter("tex", sf::Shader::CurrentTexture);
			return true;
		}
		return true;
	}
}