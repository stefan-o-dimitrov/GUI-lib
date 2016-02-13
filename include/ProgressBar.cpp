#include <SFML/Window/Event.hpp>

#include "ProgressBar.h"

const float gui::ProgressBar::PROGRESS_UPS = 10.0f;

gui::ProgressBar::ProgressBar(const Icon& background, const Icon& _fill, 
	const unsigned char _progress) : Icon(background), fill(_fill)
{
	fill.clearMessage();
	setProgress(_progress);
	setPosition(0, 0);
}

gui::ProgressBar::ProgressBar(Icon&& _background, Icon&& _fill, const unsigned char _progress)
	: Icon(std::move(_background)), fill(_fill)
{
	setProgress(_progress);
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
		Icon::input(event);
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

gui::ProgressBar& gui::ProgressBar::setProgress(float _progress)const
{
	if (_progress > 1.0f) _progress = 1.0f;
	if (_progress < 0.0f) _progress = 0.0f;

	progress = _progress;
	fill.setTextureRect(sf::IntRect(0, 0,
		fill.getTexture().getSize().x * progress,
		fill.getTexture().getSize().y));
	return (ProgressBar&)*this;
}

gui::ProgressBar& gui::ProgressBar::setFillTexture(const sf::Texture& _fill, const bool transparencyCheck)
{
	fill.setTexture(_fill, transparencyCheck);
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