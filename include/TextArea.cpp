#include "TextArea.h"
#include "Internals.h"

#include <sstream>

namespace gui
{
	TextArea::TextArea(const std::string& newText, const sf::Font& newFont, const unsigned char newCharacterSize)
		: text(newText, newFont, newCharacterSize) {}

	TextArea::TextArea(const TextArea& copy)
		: Hoverable(copy), text(copy.text) {}

	const bool TextArea::contains(const sf::Vector2f& pos) const
	{
		return text.getGlobalBounds().contains(pos);
	}

	const sf::FloatRect TextArea::getGlobalBounds()const
	{
		return text.getGlobalBounds();
	}

	const sf::Vector2f& TextArea::getPosition()const
	{
		return text.getPosition();
	}

	const sf::Font& TextArea::getFont()const
	{
		return *text.getFont();
	}

	const unsigned char TextArea::getCharacterSize()const
	{
		return text.getCharacterSize();
	}

	TextArea& TextArea::setPosition(const float x, const float y)
	{
		text.setPosition(x, y);
		return *this;
	}

	TextArea& TextArea::setText(const ColoredString& newText)const
	{
		text.setString(newText.first);
		text.setColor(newText.second);
		return (TextArea&)*this;
	}

	TextArea& TextArea::setFont(const sf::Font& newFont)
	{
		text.setFont(newFont);
		return *this;
	}

	TextArea& TextArea::setCharacterSize(const unsigned char newCharacterSize)
	{
		text.setCharacterSize(newCharacterSize);
		return *this;
	}

	void TextArea::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		if (updateFunction && Duration(Internals::timeSinceStart() - timeOfLastUpdate).count() > 1.0f / Internals::getUPS())
		{
			setText((*updateFunction)());
			timeOfLastUpdate = Internals::timeSinceStart();
		}
		target.draw(text, states);
		Hoverable::draw(target, states);
	}
}