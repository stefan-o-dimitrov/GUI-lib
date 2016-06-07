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

#include "../include/GUI/ProgressBar.h"

#include <SFML/Window/Event.hpp>

namespace gui
{
	ProgressBar::ProgressBar(const Icon& newBackground, const Icon& newFill,
		const unsigned char newProgress) : Icon(newBackground), m_fill(newFill)
	{
		m_fill.clearMessage();
		setProgress(newProgress);
		setPosition(0, 0);
	}

	ProgressBar::ProgressBar(Icon&& newBackground, Icon&& newFill, const unsigned char newProgress)
		: Icon(std::move(newBackground)), m_fill(newFill)
	{
		setProgress(newProgress);
		setPosition(0, 0);
	}

	ProgressBar::ProgressBar(const ProgressBar& copy)
		: Icon(copy),
		m_fill(copy.m_fill),
		m_updateFunction(copy.m_updateFunction ? new auto(*copy.m_updateFunction) : nullptr),
		m_progress(copy.m_progress) {}

	ProgressBar& ProgressBar::operator=(const ProgressBar& copy)
	{
		Icon::operator=(copy);
		m_fill = copy.m_fill;
		m_progress = copy.m_progress;
		m_updateFunction.reset(copy.m_updateFunction ? new auto(*copy.m_updateFunction) : nullptr);
	}

	std::unique_ptr<Interactive> ProgressBar::copy() const
	{ 
		return std::unique_ptr<ProgressBar>(new ProgressBar(*this));
	}

	std::unique_ptr<Interactive> ProgressBar::move()
	{ 
		return std::unique_ptr<ProgressBar>(new ProgressBar(std::move(*this)));
	}

	void gui::ProgressBar::lostFocus()
	{
		Icon::lostFocus();
		m_fill.lostFocus();
	}

	const bool ProgressBar::input(const sf::Event& event)
	{
		m_fill.setParentToSame(*this);
		if (m_fill.input(event))
		{
			if (!m_fill.getMessage()) Icon::input(event);
			else mouseLeft();
			return true;
		}
		else return Icon::input(event);
	}

	const bool gui::ProgressBar::contains(const float x, const float y) const
	{
		return Icon::contains(x, y) || m_fill.contains(x, y);
	}

	const float ProgressBar::getProgress()const
	{
		return m_progress;
	}

	const sf::Texture* ProgressBar::getFillTexture()const
	{
		return m_fill.getTexture();
	}

	const bool ProgressBar::getFillTransparencyCheck()const
	{
		return m_fill.getTransparencyCheck();
	}

	ProgressBar& ProgressBar::setUpdateFunction(const std::function<const float()>& function)
	{
		m_updateFunction ? *m_updateFunction = function : m_updateFunction.reset(new std::function<const float()>(function));
		return *this;
	}

	ProgressBar& ProgressBar::setUpdateFunction(std::function<const float()>&& function)
	{
		m_updateFunction ? *m_updateFunction = std::move(function) : m_updateFunction.reset(new std::function<const float()>(std::move(function)));
		return *this;
	}

	ProgressBar& ProgressBar::setProgress(float newProgress)const
	{
		if (newProgress > 1.0f) newProgress = 1.0f;
		if (newProgress < 0.0f) newProgress = 0.0f;

		m_progress = newProgress;
		m_fill.setTextureRect(sf::IntRect(0, 0,
			m_fill.getTexture() ? m_fill.getTexture()->getSize().x * m_progress : 0,
			m_fill.getTexture() ? m_fill.getTexture()->getSize().y : 0));
		return (ProgressBar&)*this;
	}

	ProgressBar& ProgressBar::setFillTexture(const sf::Texture& newTexture, const bool transparencyCheck)
	{
		m_fill.setTexture(newTexture, transparencyCheck);
		return *this;
	}

	ProgressBar& ProgressBar::setFillTransparencyCheck(const bool transparencyCheck)
	{
		m_fill.setTransparencyCheck(transparencyCheck);
		return *this;
	}

	ProgressBar& ProgressBar::setPosition(const float x, const float y)
	{
		Icon::setPosition(x, y);
		m_fill.setPosition(x + ((Icon::getTexture() ? Icon::getTexture()->getSize().x : 0) - (m_fill.getTexture() ? m_fill.getTexture()->getSize().x : 0)) / 2.0f,
			y + ((Icon::getTexture() ? Icon::getTexture()->getSize().y : 0) - (m_fill.getTexture() ? m_fill.getTexture()->getSize().y : 0)) / 2.0f);
		return *this;
	}

	ProgressBar& ProgressBar::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	const std::shared_ptr<const HoverMessage> ProgressBar::getFillMessage() const
	{
		return m_fill.getMessage();
	}

	ProgressBar& ProgressBar::clearFillMessage()
	{
		m_fill.clearMessage();
		return *this;
	}

	ProgressBar& ProgressBar::setFillMessage(const HoverMessage& message)
	{
		m_fill.setMessage(message);
		return *this;
	}

	ProgressBar& ProgressBar::setFillMessage(HoverMessage&& messageTemp)
	{
		m_fill.setMessage(std::move(messageTemp));
		return *this;
	}

	ProgressBar& ProgressBar::setFillDelay(const float delaySeconds)
	{
		m_fill.setDelay(delaySeconds);
		return *this;
	}

	void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_updateFunction && Duration(Internals::timeSinceStart() - m_timeOfLastUpdate).count() > 1.0f / Internals::getUPS())
		{
			setProgress((*m_updateFunction)());
			m_timeOfLastUpdate = Internals::timeSinceStart();
		}

		target.draw(Icon::m_icon, states);
		target.draw(m_fill, states);
		if (!m_fill.getMessage() || !m_fill.hasMessageDelayPassed()) Hoverable::draw(target, states);
	}
}