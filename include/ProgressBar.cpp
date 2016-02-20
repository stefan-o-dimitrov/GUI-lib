#include <SFML/Window/Event.hpp>

#include "ProgressBar.h"

const float gui::ProgressBar::PROGRESS_UPS = 10.0f;

gui::ProgressBar::ProgressBar(const Icon& newBackground, const Icon& newFill,
	const unsigned char newProgress) : Icon(newBackground), fill(newFill)
{
	fill.clearMessage();
	setProgress(newProgress);
	setPosition(0, 0);
}

gui::ProgressBar::ProgressBar(Icon&& newBackground, Icon&& newFill, const unsigned char newProgress)
	: Icon(std::move(newBackground)), fill(newFill)
{
	setProgress(newProgress);
	setPosition(0, 0);
}

gui::ProgressBar::ProgressBar(const ProgressBar& copy)
	: Icon(copy),
	fill(copy.fill),
	updateFunction(copy.updateFunction ? new std::function<const float()>(*copy.updateFunction) : nullptr),
	progress(copy.progress) {}

const bool gui::ProgressBar::input(const sf::Event& event)
{
	if (fill.input(event))
	{
		if (!fill.getMessage()) Icon::input(event);
		else mouseLeft();
		return true;
	}
	else return Icon::input(event);
}

const float gui::ProgressBar::getProgress()const
{
	return progress;
}

const sf::Texture& gui::ProgressBar::getFillTexture()const
{
	return fill.getTexture();
}

const bool gui::ProgressBar::getFillTransparencyCheck()const
{
	return fill.getTransparencyCheck();
}

gui::ProgressBar& gui::ProgressBar::setUpdateFunction(const std::function<const float()>& function)
{
	updateFunction ? *updateFunction = function : updateFunction.reset(new std::function<const float()>(function));
	return *this;
}

gui::ProgressBar& gui::ProgressBar::setUpdateFunction(std::function<const float()>&& function)
{
	updateFunction ? *updateFunction = std::move(function) : updateFunction.reset(new std::function<const float()>(std::move(function)));
	return *this;
}

gui::ProgressBar& gui::ProgressBar::setProgress(float newProgress)const
{
	if (newProgress > 1.0f) newProgress = 1.0f;
	if (newProgress < 0.0f) newProgress = 0.0f;

	progress = newProgress;
	fill.setTextureRect(sf::IntRect(0, 0,
		fill.getTexture().getSize().x * progress,
		fill.getTexture().getSize().y));
	return (ProgressBar&)*this;
}

gui::ProgressBar& gui::ProgressBar::setFillTexture(const sf::Texture& newTexture, const bool transparencyCheck)
{
	fill.setTexture(newTexture, transparencyCheck);
	return *this;
}

gui::ProgressBar& gui::ProgressBar::setFillTransparencyCheck(const bool transparencyCheck)
{
	fill.setTransparencyCheck(transparencyCheck);
	return *this;
}

gui::ProgressBar& gui::ProgressBar::setPosition(const float x, const float y)
{
	Icon::setPosition(x, y);
	fill.setPosition(x - fill.getGlobalBounds().width / 2.0f,
		y + Icon::getGlobalBounds().height / 2.0f - fill.getGlobalBounds().height / 2.0f);
	return *this;
}

gui::ProgressBar& gui::ProgressBar::setPosition(const sf::Vector2f& pos)
{
	return setPosition(pos.x, pos.y);
}

const std::shared_ptr<const gui::HoverMessage> gui::ProgressBar::getFillMessage() const
{
	return fill.getMessage();
}

gui::ProgressBar& gui::ProgressBar::clearFillMessage()
{
	fill.clearMessage();
}

gui::ProgressBar& gui::ProgressBar::setFillMessage(const HoverMessage& message)
{
	fill.setMessage(message);
	return *this;
}

gui::ProgressBar& gui::ProgressBar::setFillMessage(HoverMessage&& messageTemp)
{
	fill.setMessage(std::move(messageTemp));
	return *this;
}

gui::ProgressBar& gui::ProgressBar::setFillDelay(const float delaySeconds)
{
	fill.setDelay(delaySeconds);
}

void gui::ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (updateFunction && clock.getElapsedTime().asSeconds() > timeOfLastUpdate + (1.0f / PROGRESS_UPS))
	{
		setProgress((*updateFunction)());
		timeOfLastUpdate = clock.getElapsedTime().asSeconds();
	}

	Icon::draw(target, states);
	target.draw(fill, states);
}