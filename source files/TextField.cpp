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

#include "../include/GUI/TextField.h"

namespace gui
{
	TextField::TextField(const std::function<void(const std::string&)>& processTextInputFunction, const unsigned short fieldWidth, const sf::Font& font, const unsigned char characterSize)
		: processingFunction(processTextInputFunction), currentInput("", font, characterSize)
	{
		box.reset(sf::FloatRect(0, 0, fieldWidth, sf::Text("I", font, characterSize).getGlobalBounds().height));
	}

	TextField::TextField(const TextField& source)
		: box(source.box), currentInput(source.currentInput), processingFunction(source.processingFunction), allowClipboard(source.allowClipboard)
	{
		if (source.prompt) prompt.reset(new sf::Text(*source.prompt));
	}

	std::unique_ptr<Interactive> TextField::copy() const
	{
		return std::unique_ptr<TextField>(new TextField(*this));
	}

	std::unique_ptr<Interactive> TextField::move()
	{ 
		return std::unique_ptr<TextField>(new TextField(std::move(*this)));
	}

	const bool TextField::input(const sf::Event& event)
	{
		switch (event.type)
		{
		case sf::Event::MouseMoved:
			break;
		case sf::Event::MouseButtonPressed:
			break;
		case sf::Event::MouseButtonReleased:
			break;
		case sf::Event::TextEntered:
			break;
		case sf::Event::KeyPressed:
			break;
		case sf::Event::LostFocus:
			break;
		default:
			return false;
		}
	}

	void TextField::processCurrentInput()
	{
		processingFunction(currentInput.getString());
		currentInput.setString("");
		cursorPosition = 0;
		active = false;
	}

	const sf::Vector2f& TextField::getPosition() const
	{
		return sf::Vector2f();
	}

	const sf::FloatRect TextField::getGlobalBounds() const
	{
		return sf::FloatRect();
	}

	const std::string& TextField::getCurrentInput()const
	{
		return currentInput.getString();
	}

	const unsigned char TextField::getCharacterSize()const
	{
		return currentInput.getCharacterSize();
	}

	const sf::Color& TextField::getTextColor()const
	{
		return currentInput.getColor();
	}

	TextField& TextField::setCharacterSize(const unsigned char characterSize)
	{
		currentInput.setCharacterSize(characterSize);
		box.setSize(box.getSize().x, sf::Text("|", *currentInput.getFont(), characterSize).getGlobalBounds().height);
		return *this;
	}

	TextField& TextField::setTextColor(const sf::Color& color)
	{
		currentInput.setColor(color);
		return *this;
	}

	TextField& TextField::setPrompt(const std::string& str, const sf::Color& color)
	{
		if (!prompt) prompt.reset(new sf::Text(str, *currentInput.getFont(), currentInput.getCharacterSize()));
		else prompt->setString(str);
		prompt->setColor(color);
		prompt->setPosition(position);
		return *this;
	}

	TextField& TextField::setPromptColor(const sf::Color& color)
	{
		if (!prompt) prompt.reset(new sf::Text("", *currentInput.getFont(), currentInput.getCharacterSize()));
		prompt->setColor(color);
		return *this;
	}

	TextField& TextField::clearPrompt()
	{
		prompt.reset();
		return *this;
	}

	TextField& TextField::setPosition(const float x, const float y)
	{
		position.x = x;
		position.y = y;
		if (prompt) prompt->setPosition(x, y);
		return *this;
	}

	TextField& TextField::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	TextField& TextField::setFont(const sf::Font& font)
	{
		currentInput.setFont(font);
		if (prompt) prompt->setFont(font);
		return *this;
	}

	TextField& TextField::setFieldWidth(const unsigned short width)
	{
		box.reset(sf::FloatRect(0, 0, width, box.getSize().y));
		return *this;
	}

	TextField& TextField::setClipboardPermission(const bool permission)
	{
		allowClipboard = permission;
		return *this;
	}

	void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (prompt && currentInput.getString().isEmpty())
		{
			target.draw(*prompt, states);
			return;
		}
		box.setViewport(sf::FloatRect(position.x / target.getSize().x, position.y / target.getSize().y, box.getSize().x / target.getSize().x, box.getSize().y / target.getSize().y));
		const sf::View buffer = target.getView();
		target.setView(box);
		target.draw(currentInput);
		target.setView(buffer);
	}
}