#include <sstream>
#include <iostream>

#include "TextPane.h"
#include "HoverMessage.h"
#include "Internals.h"


gui::TextPane::TextPane(const ColoredString & string, const sf::Font & font, const unsigned char characterSize)
	: font(&font), characterSize(characterSize)
{
	setText(string);
}

gui::TextPane::TextPane(const TextPane& copy)
	: position(copy.position)
{
	for (auto it = copy.text.begin(), end = copy.text.end(); it != end; ++it)
		text.push_back(std::unique_ptr<sf::Text>(new sf::Text(*(*it))));
}

const bool gui::TextPane::contains(const sf::Vector2f& pos) const
{
	for (auto it = text.begin(), end = text.end(); it != end; ++it)
		if ((*it)->getGlobalBounds().contains(pos)) return true;
	return false;
}

const sf::FloatRect gui::TextPane::getGlobalBounds()const
{
	sf::FloatRect returnValue(0, 0, 0, 0);
	for (auto it = text.begin(), end = text.end(); it != end; ++it)
	{
		if (returnValue.width < (*it)->getGlobalBounds().width + (*it)->getPosition().x) returnValue.width = (*it)->getGlobalBounds().width + (*it)->getPosition().x;
		if (returnValue.height < (*it)->getGlobalBounds().height + (*it)->getPosition().y) returnValue.height = (*it)->getGlobalBounds().height + (*it)->getPosition().y;
	}
	returnValue.left = position.x;
	returnValue.top = position.y;
	
	return returnValue;
}

const sf::Vector2f& gui::TextPane::getPosition()const
{
	return position;
}

const sf::Font& gui::TextPane::getFont()const
{
	return *font;
}

const unsigned char gui::TextPane::getCharacterSize()const
{
	return characterSize;
}

gui::TextPane& gui::TextPane::setPosition(const float x, const float y)
{
	position.x = x;
	position.y = y;
	return *this;
}

gui::TextPane& gui::TextPane::setText(const ColoredString& string)
{
	string.getText(text, *font, characterSize);
	return *this;
}

gui::TextPane& gui::TextPane::setFont(const sf::Font& newFont)
{
	font = &newFont;
	for (auto it = text.begin(), end = text.end(); it != end; ++it)
		(*it)->setFont(newFont);
	return *this;
}

gui::TextPane& gui::TextPane::setCharacterSize(const unsigned char newCharacterSize)
{
	characterSize = newCharacterSize;
	for (auto it = text.begin(), end = text.end(); it != end; ++it)
		(*it)->setCharacterSize(characterSize);
	ColoredString::arrangeText(text);
	return *this;
}

void gui::TextPane::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	states.transform.translate(position);
	for (auto it = text.begin(), end = text.end(); it != end; ++it)
		target.draw(**it, states);
}