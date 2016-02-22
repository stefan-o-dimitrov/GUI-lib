#include "TextArea.h"
#include "Internals.h"

#include <sstream>

namespace gui
{
	TextArea::TextArea(const std::string& newText, const sf::Font& newFont, const unsigned char newCharacterSize)
		: text(newText, newFont, newCharacterSize) {}

	TextArea::TextArea(const TextArea& copy)
		: Hoverable(copy), text(copy.text) {}

	std::unique_ptr<Interactive> TextArea::copy() const
	{
		return std::unique_ptr<TextArea>(new TextArea(*this));
	}

	std::unique_ptr<Interactive> TextArea::move()
	{ 
		return std::unique_ptr<TextArea>(new TextArea(std::move(*this)));
	}

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

	TextArea& TextArea::clearMessage()
	{
		Hoverable::clearMessage();
		return *this;
	}

	TextArea& TextArea::setMessage(const HoverMessage& newMessage)
	{
		Hoverable::setMessage(newMessage);
		return *this;
	}

	TextArea& TextArea::setMessage(HoverMessage&& tempMessage)
	{
		Hoverable::setMessage(std::move(tempMessage));
		return *this;
	}

	TextArea& TextArea::setPosition(const float x, const float y)
	{
		text.setPosition(x, y);
		return *this;
	}

	TextArea& TextArea::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
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

	TextArea& TextArea::setColor(const sf::Color& color)
	{ 
		text.setColor(color);
		return *this;
	}

	TextArea& TextArea::setUpdateFunction(const std::function<ColoredString()>& func)
	{
		updateFunction.reset(new std::function<ColoredString()>(func));
		return *this;
	}

	TextArea& TextArea::setUpdateFunction(std::function<ColoredString()>&& func)
	{ 
		updateFunction.reset(new std::function<ColoredString()>(std::move(func)));
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