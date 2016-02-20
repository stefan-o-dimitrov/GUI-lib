#include <sstream>

#include "TextArea.h"
#include "Internals.h"

const unsigned char gui::TextArea::TEXT_UPS = 10;

gui::TextArea::TextArea(const std::string& newText, const sf::Font& newFont, const unsigned char newCharacterSize)
	: text(newText, newFont, newCharacterSize) {}

gui::TextArea::TextArea(const TextArea& copy)
	: Hoverable(copy), text(copy.text) {}

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

gui::TextArea& gui::TextArea::setText(const cString& newText)const
{
	text.setString(newText.first);
	text.setColor(newText.second);
	return (TextArea&)*this;
}

gui::TextArea& gui::TextArea::setFont(const sf::Font& newFont)
{
	text.setFont(newFont);
	return *this;
}

gui::TextArea& gui::TextArea::setCharacterSize(const unsigned char newCharacterSize)
{
	text.setCharacterSize(newCharacterSize);
	return *this;
}

void gui::TextArea::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	if (updateFunction && clock.getElapsedTime().asSeconds() > timeSinceUpdate + (1.0f / TEXT_UPS))
	{
		setText((*updateFunction)());
		timeSinceUpdate = clock.getElapsedTime().asSeconds();
	}
	target.draw(text, states);
	Hoverable::draw(target, states);
}
