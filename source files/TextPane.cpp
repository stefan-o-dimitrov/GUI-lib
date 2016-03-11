////////////////////////////////////////////////////////////
//
// GUI-lib - Graphical User Interface Library
// Copyright (C) 2014-2016 ShtGames (shtgamessts@gmail.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#include "../include/GUI/TextPane.h"
#include "../include/GUI/HoverMessage.h"
#include "../include/GUI/Internals.h"

#include <sstream>

namespace gui
{
	TextPane::TextPane(const ColoredText& newString, const sf::Font& newFont, const unsigned char newCharacterSize)
		: characterSize(newCharacterSize), string(newString)
	{
		setFont(newFont);
		update();
	}

	TextPane::TextPane(ColoredText&& newString, const sf::Font& newFont, const unsigned char newCharacterSize)
		: characterSize(newCharacterSize), string(std::move(newString))
	{
		setFont(newFont);
		update();
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
		return *this;
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
		return (event.type == sf::Event::MouseMoved && contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) ||
			((event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) &&
				contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)));
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
		update();
		return *this;
	}

	TextPane& TextPane::setText(ColoredText&& newText)
	{
		string = std::move(newText);
		update();
		return *this;
	}

	TextPane& TextPane::setFont(const sf::Font& newFont)
	{
		font = &newFont;
		for (auto it = text.begin(), end = text.end(); it != end; ++it)
			(*it)->setFont(*font);
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
			update();
		states.transform.translate(position);
		for (auto it = text.begin(), end = text.end(); it != end; ++it)
			target.draw(**it, states);
	}

	void TextPane::update() const
	{
		string.getText(text, text.size() != 0 ? *text.front()->getFont() : *font, characterSize);
		timeOfLastUpdate = Internals::timeSinceStart();
	}
}