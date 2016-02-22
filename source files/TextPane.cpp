#include "../include/GUI/TextPane.h"
#include "../include/GUI/HoverMessage.h"
#include "../include/GUI/Internals.h"

#include <sstream>
#include <iostream>

namespace gui
{
	TextPane::TextPane(const ColoredText& newString, const sf::Font& newFont, const unsigned char newCharacterSize)
		: font(&newFont), characterSize(newCharacterSize), string(newString)
	{
		setText(newString);
	}

	TextPane::TextPane(const TextPane& copy)
		: position(copy.position), string(copy.string)
	{
		for (auto it = copy.text.begin(), end = copy.text.end(); it != end; ++it)
			text.push_back(std::unique_ptr<sf::Text>(new sf::Text(*(*it))));
	}

	TextPane& TextPane::operator=(const TextPane& copy)
	{
		position = copy.position;
		string = copy.string;
		for (auto it = copy.text.begin(), end = copy.text.end(); it != end; ++it)
			text.push_back(std::unique_ptr<sf::Text>(new sf::Text(*(*it))));
	}

	std::unique_ptr<Interactive> TextPane::copy() const
	{
		return std::unique_ptr<TextPane>(new TextPane(*this));
	}

	std::unique_ptr<Interactive> TextPane::move()
	{ 
		return std::unique_ptr<TextPane>(new TextPane(std::move(*this)));
	}

	const bool TextPane::input(const sf::Event& event)
	{
		return false;
	}

	const bool TextPane::contains(const sf::Vector2f& pos) const
	{
		for (auto it = text.begin(), end = text.end(); it != end; ++it)
			if ((*it)->getGlobalBounds().contains(pos)) return true;
		return false;
	}

	const sf::FloatRect TextPane::getGlobalBounds()const
	{
		sf::FloatRect returnValue(0, 0, 0, 0);
		for (auto it = text.begin(), end = text.end(); it != end; ++it)
		{
			if (returnValue.width < (*it)->getGlobalBounds().width + (*it)->getPosition().x)
				returnValue.width = (*it)->getGlobalBounds().width + (*it)->getPosition().x;
			if (returnValue.height < (*it)->getGlobalBounds().height + (*it)->getPosition().y)
				returnValue.height = (*it)->getGlobalBounds().height + (*it)->getPosition().y;
		}
		returnValue.left = position.x;
		returnValue.top = position.y;

		return returnValue;
	}

	const sf::Vector2f& TextPane::getPosition()const
	{
		return position;
	}

	const sf::Font& TextPane::getFont()const
	{
		return *font;
	}

	const unsigned char TextPane::getCharacterSize()const
	{
		return characterSize;
	}

	TextPane& TextPane::setPosition(const float x, const float y)
	{
		position.x = x;
		position.y = y;
		return *this;
	}

	TextPane& TextPane::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	TextPane& TextPane::setText(const ColoredText& newText)
	{
		string = newText;
		newText.getText(text, *font, characterSize);
		return *this;
	}

	TextPane& TextPane::setText(ColoredText&& newText)
	{
		string = std::move(newText);
		newText.getText(text, *font, characterSize);
		return *this;
	}

	TextPane& TextPane::setFont(const sf::Font& newFont)
	{
		font = &newFont;
		for (auto it = text.begin(), end = text.end(); it != end; ++it)
			(*it)->setFont(newFont);
		return *this;
	}

	TextPane& TextPane::setCharacterSize(const unsigned char newCharacterSize)
	{
		characterSize = newCharacterSize;
		update();
		return *this;
	}

	void TextPane::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		if (string.isVolatile() && Duration(Internals::timeSinceStart() - timeOfLastUpdate).count() > 1.0f / Internals::getUPS())
		{
			update();
			timeOfLastUpdate = Internals::timeSinceStart();
		}
		states.transform.translate(position);
		for (auto it = text.begin(), end = text.end(); it != end; ++it)
			target.draw(**it, states);
	}

	void TextPane::update() const
	{
		string.getText(text, *font, characterSize);
	}
}