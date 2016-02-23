#include "../include/GUI/ProgressBar.h"

#include <SFML/Window/Event.hpp>

namespace gui
{
	ProgressBar::ProgressBar(const Icon& newBackground, const Icon& newFill,
		const unsigned char newProgress) : Icon(newBackground), fill(newFill)
	{
		fill.clearMessage();
		setProgress(newProgress);
		setPosition(0, 0);
	}

	ProgressBar::ProgressBar(Icon&& newBackground, Icon&& newFill, const unsigned char newProgress)
		: Icon(std::move(newBackground)), fill(newFill)
	{
		setProgress(newProgress);
		setPosition(0, 0);
	}

	ProgressBar::ProgressBar(const ProgressBar& copy)
		: Icon(copy),
		fill(copy.fill),
		updateFunction(copy.updateFunction ? new std::function<const float()>(*copy.updateFunction) : nullptr),
		progress(copy.progress) {}

	std::unique_ptr<Interactive> ProgressBar::copy() const
	{ 
		return std::unique_ptr<ProgressBar>(new ProgressBar(*this));
	}

	std::unique_ptr<Interactive> ProgressBar::move()
	{ 
		return std::unique_ptr<ProgressBar>(new ProgressBar(std::move(*this)));
	}

	const bool ProgressBar::input(const sf::Event& event)
	{
		if (fill.input(event))
		{
			if (!fill.getMessage()) Icon::input(event);
			else mouseLeft();
			return true;
		}
		else return Icon::input(event);
	}

	const float ProgressBar::getProgress()const
	{
		return progress;
	}

	const sf::Texture& ProgressBar::getFillTexture()const
	{
		return fill.getTexture();
	}

	const bool ProgressBar::getFillTransparencyCheck()const
	{
		return fill.getTransparencyCheck();
	}

	ProgressBar& ProgressBar::setUpdateFunction(const std::function<const float()>& function)
	{
		updateFunction ? *updateFunction = function : updateFunction.reset(new std::function<const float()>(function));
		return *this;
	}

	ProgressBar& ProgressBar::setUpdateFunction(std::function<const float()>&& function)
	{
		updateFunction ? *updateFunction = std::move(function) : updateFunction.reset(new std::function<const float()>(std::move(function)));
		return *this;
	}

	ProgressBar& ProgressBar::setProgress(float newProgress)const
	{
		if (newProgress > 1.0f) newProgress = 1.0f;
		if (newProgress < 0.0f) newProgress = 0.0f;

		progress = newProgress;
		fill.setTextureRect(sf::IntRect(0, 0,
			fill.getTexture().getSize().x * progress,
			fill.getTexture().getSize().y));
		return (ProgressBar&)*this;
	}

	ProgressBar& ProgressBar::setFillTexture(const sf::Texture& newTexture, const bool transparencyCheck)
	{
		fill.setTexture(newTexture, transparencyCheck);
		return *this;
	}

	ProgressBar& ProgressBar::setFillTransparencyCheck(const bool transparencyCheck)
	{
		fill.setTransparencyCheck(transparencyCheck);
		return *this;
	}

	ProgressBar& ProgressBar::setPosition(const float x, const float y)
	{
		Icon::setPosition(x, y);
		fill.setPosition(x - fill.getGlobalBounds().width / 2.0f,
			y + Icon::getGlobalBounds().height / 2.0f - fill.getGlobalBounds().height / 2.0f);
		return *this;
	}

	ProgressBar& ProgressBar::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}
	
	const std::shared_ptr<const HoverMessage> ProgressBar::getFillMessage() const
	{
		return fill.getMessage();
	}

	ProgressBar& ProgressBar::clearFillMessage()
	{
		fill.clearMessage();
	}

	ProgressBar& ProgressBar::setFillMessage(const HoverMessage& message)
	{
		fill.setMessage(message);
		return *this;
	}

	ProgressBar& ProgressBar::setFillMessage(HoverMessage&& messageTemp)
	{
		fill.setMessage(std::move(messageTemp));
		return *this;
	}

	ProgressBar& ProgressBar::setFillDelay(const float delaySeconds)
	{
		fill.setDelay(delaySeconds);
	}

	void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (updateFunction && Duration(Internals::timeSinceStart() - timeOfLastUpdate).count() > 1.0f / Internals::getUPS())
		{
			setProgress((*updateFunction)());
			timeOfLastUpdate = Internals::timeSinceStart();
		}

		target.draw(Icon::spr, states);
		target.draw(fill, states);
		Hoverable::draw(target, states);
	}
}