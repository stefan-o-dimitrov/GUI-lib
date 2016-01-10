#include <sstream>

#include "TextArea.h"
#include "Internals.h"

gui::TextArea::TextArea(const ColoredString& _text, const sf::Font& _font, const unsigned char _characterSize)
: text((*(*_text.vec.begin())).first, _font, _characterSize)
{
	text.setColor((*(*_text.vec.begin())).second);
}

gui::TextArea::TextArea(const TextArea& _lVal)
	: text(_lVal.text)
{
	Hoverable::operator=(_lVal);
}

gui::TextArea& gui::TextArea::operator=(const TextArea& _lVal)
{
	Hoverable::operator=(_lVal);
	text = _lVal.text;
	return *this;
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
	target.draw(text, states);
	Hoverable::draw(target, states);
}
