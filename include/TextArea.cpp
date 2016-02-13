#include <sstream>

#include "TextArea.h"
#include "Internals.h"

const unsigned char gui::TextArea::TEXT_UPS = 10;

gui::TextArea::TextArea(const std::string& _text, const sf::Font& _font, const unsigned char _characterSize)
	: text(_text, _font, _characterSize) {}

gui::TextArea::TextArea(const TextArea& _lVal)
	: text(_lVal.text)
{
	Hoverable::operator=(_lVal);
}

const bool gui::TextArea::contains(const sf::Vector2f& pos) const
{
	return text.getGlobalBounds().contains(pos);
}

const sf::FloatRect gui::TextArea::getGlobalBounds()const
{
	return text.getGlobalBounds();
}

const sf::Vector2f& gui::TextArea::getPosition()const
{
	return text.getPosition();
}

const sf::Font& gui::TextArea::getFont()const
{
	return *text.getFont();
}

const unsigned char gui::TextArea::getCharacterSize()const
{
	return text.getCharacterSize();
}

gui::TextArea& gui::TextArea::setPosition(const float x, const float y)
{
	text.setPosition(x, y);
	return *this;
}

gui::TextArea& gui::TextArea::setText(const ColoredString& _text)
{
	text.setString((*(*_text.vec.begin())).first);
	text.setColor((*(*_text.vec.begin())).second);
	return *this;
}

gui::TextArea& gui::TextArea::setFont(const sf::Font& _font)
{
	text.setFont(_font);
	return *this;
}

gui::TextArea& gui::TextArea::setCharacterSize(const unsigned char _characterSize)
{
	text.setCharacterSize(_characterSize);
	return *this;
}

void gui::TextArea::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (updateFunction && clock.getElapsedTime().asSeconds() > timeSinceUpdate + (1.0f / TEXT_UPS))
	{
		text.setString((*updateFunction)());
		timeSinceUpdate = clock.getElapsedTime().asSeconds();
	}
	target.draw(text, states);
	Hoverable::draw(target, states);
}
