#include "ProgressBar.h"

gui::ProgressBar::ProgressBar(const Icon& background, const Icon& _fill, 
	const unsigned char _progress) : Icon(background), fill(_fill)
{
	setProgress(_progress);
	setPosition(0, 0);
}

gui::ProgressBar::ProgressBar(Icon&& _background, Icon&& _fill, const unsigned char _progress)
	: Icon(std::move(_background)), fill(_fill)
{
	setProgress(_progress);
	setPosition(0, 0);
}

const bool gui::ProgressBar::input(const sf::Event& event)
{
	if (!fill.input(event))
		return Icon::input(event);
	else return true;
}

const unsigned char gui::ProgressBar::getProgress()const
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

const std::shared_ptr<const gui::HoverMessage> gui::ProgressBar::getFillMessage()const
{
	return fill.getMessage();
}

gui::ProgressBar& gui::ProgressBar::setProgress(const unsigned char _progress)
{
	if (_progress > 100)
		return *this;
	progress = _progress;
	fill.setTextureRect(sf::IntRect(0, 0,
		fill.getTexture().getSize().x * (progress / 100.0f),
		fill.getTexture().getSize().y));
	return *this;
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
	fill.setPosition(Icon::getPosition().x + Icon::getGlobalBounds().width / 2 - fill.getGlobalBounds().width / 2,
		Icon::getPosition().y + Icon::getGlobalBounds().height / 2 - fill.getGlobalBounds().height / 2);
	return *this;
}

gui::ProgressBar& gui::ProgressBar::setFillMessage(const HoverMessage& _message)
{
	fill.setMessage(_message);
	return *this;
}

gui::ProgressBar& gui::ProgressBar::setFillMessage(HoverMessage&& _message)
{
	fill.setMessage(std::move(_message));
	return *this;
}

void gui::ProgressBar::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	Icon::draw(target, states);
	target.draw(fill, states);
}