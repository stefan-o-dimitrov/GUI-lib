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
	TextField::TextField(const sf::Font& font, const unsigned short fieldWidth, const unsigned char characterSize)
		: m_input("", font, characterSize), m_cursor("|", font, characterSize)
	{
		m_box.reset(sf::FloatRect(-3, 0, fieldWidth + 6, m_cursor.getGlobalBounds().height * 2));
	}

	TextField::TextField(const TextField& source)
		: m_position(source.m_position), m_input(source.m_input), m_cursor(source.m_cursor),
		m_processingFunction(source.m_processingFunction),
		m_clearAfterInputProcessed(source.m_clearAfterInputProcessed), m_box(source.m_box)
	{
		if (source.m_prompt) m_prompt.reset(new auto(*source.m_prompt));
	}

	TextField::TextField()
	{
		m_cursor.setString("|");
	}

	TextField& TextField::operator=(const TextField& source)
	{
		m_box = source.m_box;
		m_input = source.m_input;
		m_processingFunction = source.m_processingFunction;
		m_position = source.m_position;
		m_cursor = source.m_cursor;
		m_clearAfterInputProcessed = source.m_clearAfterInputProcessed;
		if (source.m_prompt) m_prompt.reset(new auto(*source.m_prompt));
		return *this;
	}

	std::unique_ptr<Interactive> TextField::copy() const
	{
		return std::unique_ptr<TextField>(new TextField(*this));
	}

	std::unique_ptr<Interactive> TextField::move()
	{ 
		return std::unique_ptr<TextField>(new TextField(std::move(*this)));
	}

	bool returnGuard = false;

	const bool TextField::input(const sf::Event& event)
	{
		switch (event.type)
		{
			case sf::Event::MouseMoved:
				return getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);
			case sf::Event::MouseButtonPressed:
				return (getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) || m_active);
			case sf::Event::MouseButtonReleased:
			{
				if (getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
				{
					getClickedCharacter(event.mouseButton.x, event.mouseButton.y);
					return m_active = true;
				}
				else if (m_active) return !(m_active = false);
				break;
			}
			case sf::Event::TextEntered:
			{
				if (!m_active) return false;
				if (returnGuard) return !(returnGuard = false);
				if (event.text.unicode == '\b')
				{
					removeCharacter();
					return true;
				}
				addCharacter(event.text.unicode);
				return true;
			}
			case sf::Event::KeyPressed:
			{
				if (!m_active) return false;

				if (event.key.code == sf::Keyboard::Return)
				{
					returnGuard = true;
					m_active = false;
					processCurrentInput();
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					if (m_cursorPosition != 0)
						setCursorPosition(m_cursorPosition - 1);
					else m_cursorVisible = true;
				}
				else if (event.key.code == sf::Keyboard::Right)
					setCursorPosition(m_cursorPosition + 1);
				else if (event.key.code == sf::Keyboard::Delete)
					removeCharacter(false);
				else if (event.key.code == sf::Keyboard::Escape)
					m_active = false;
				return true;
			}
			case sf::Event::LostFocus:
				lostFocus();
			default: return false;
		}
		return false;
	}

	void TextField::lostFocus()
	{
		m_active = false;
	}

	void TextField::clear()
	{
		m_input.setString("");
		m_input.setPosition(0, 0);
		setCursorPosition(0);
	}

	void TextField::processCurrentInput()
	{
		if(m_processingFunction) m_processingFunction(m_input.getString());
		if (m_clearAfterInputProcessed)	clear();
	}

	void TextField::setCursorPosition(size_t position)
	{
		m_cursorVisible = true;
		auto pos(m_input.findCharacterPos(m_cursorPosition = (position > m_input.getString().getSize() ? m_input.getString().getSize() :
			position)));
		(pos.x /= 2) += m_cursorPosition != 0 ? m_input.findCharacterPos(m_cursorPosition - 1).x / 2.f : -m_cursor.getGlobalBounds().width / 2;
		if (pos.x < 0)
		{
			m_input.setPosition(int(m_cursorPosition != 0 ? m_input.getPosition().x - pos.x : 0), 0);
			pos.x = m_cursorPosition != 0 ? 0 : -2 * m_cursor.getGlobalBounds().width;
		}
		else if (pos.x + m_cursor.getGlobalBounds().width > m_box.getSize().x)
		{
			m_input.setPosition(int(m_input.getPosition().x  - pos.x + m_box.getSize().x - m_cursor.getGlobalBounds().width), 0);
			pos.x = m_box.getSize().x - m_cursor.getGlobalBounds().width;
		}

		m_cursor.setPosition(pos.x - m_cursor.getGlobalBounds().width / 2, pos.y);
	}

	const sf::Vector2f& TextField::getPosition() const
	{
		return m_position;
	}

	const sf::FloatRect TextField::getGlobalBounds() const
	{
		return sf::FloatRect(m_position, m_box.getSize());
	}

	const sf::String& TextField::getCurrentInput()const
	{
		return m_input.getString();
	}

	const unsigned char TextField::getCharacterSize()const
	{
		return m_input.getCharacterSize();
	}

	const sf::Color& TextField::getTextColor()const
	{
		return m_input.getFillColor();
	}

	size_t TextField::getCursorPosition() const
	{
		return m_cursorPosition;
	}

	const float TextField::getHeight() const
	{
		return m_cursor.getLocalBounds().height;
	}

	TextField& TextField::setCharacterSize(const unsigned char characterSize)
	{
		m_input.setCharacterSize(characterSize);
		m_cursor.setCharacterSize(characterSize);
		m_prompt->setCharacterSize(characterSize);
		m_box.setSize(m_box.getSize().x, m_cursor.getGlobalBounds().height * 2);

		return *this;
	}

	TextField& TextField::setColor(const sf::Color& color)
	{
		m_input.setFillColor(color);
		m_cursor.setFillColor(color);
		return *this;
	}

	TextField& TextField::setCursorColor(const sf::Color& color)
	{
		m_cursor.setFillColor(color);
		return *this;
	}

	TextField& TextField::setPrompt(const ColoredString& prompt)
	{
		if (!m_prompt) m_prompt.reset(new sf::Text(prompt.first, *m_input.getFont(), m_input.getCharacterSize()));
		else m_prompt->setString(prompt.first);
		m_prompt->setOutlineColor(sf::Color(15, 15, 15, 230));
		m_prompt->setFillColor(prompt.second.first);
		m_prompt->setStyle(prompt.second.second);
		m_prompt->setPosition(m_position);
		m_prompt->setCharacterSize(m_input.getCharacterSize());

		return *this;
	}

	TextField& TextField::setPromptColor(const sf::Color& color)
	{
		if (!m_prompt) m_prompt.reset(new sf::Text("", *m_input.getFont(), m_input.getCharacterSize()));
		m_prompt->setFillColor(color);
		return *this;
	}

	TextField& TextField::setPromptStyle(const sf::Text::Style style)
	{
		if (!m_prompt) m_prompt.reset(new sf::Text("", *m_input.getFont(), m_input.getCharacterSize()));
		m_prompt->setStyle(style);
		return *this;
	}

	TextField& TextField::clearPrompt()
	{
		m_prompt.reset();
		return *this;
	}

	TextField& TextField::setPosition(const float x, const float y)
	{
		m_position.x = x;
		m_position.y = y;
		if (m_prompt) m_prompt->setPosition(int(x), int(y + m_prompt->getGlobalBounds().height * 0.25f));
		return *this;
	}

	TextField& TextField::setPosition(const sf::Vector2f& newPosition)
	{
		return setPosition(newPosition.x, newPosition.y);
	}

	TextField& TextField::setInputProcessingFunction(const std::function<void(const sf::String&)>& function)
	{
		m_processingFunction = function;
		return *this;
	}

	TextField& TextField::setInputProcessingFunction(std::function<void(const sf::String&)>&& function)
	{
		m_processingFunction = std::move(function);
		return *this;
	}

	TextField& TextField::setFont(const sf::Font& font)
	{
		m_input.setFont(font);
		m_cursor.setFont(font);
		if (m_prompt) m_prompt->setFont(font);
		m_box.reset(sf::FloatRect(0, 0, m_box.getSize().x, m_cursor.getGlobalBounds().height * 2));
		return *this;
	}

	TextField& TextField::setWidth(const unsigned short width)
	{
		m_box.reset(sf::FloatRect(0, 0, width, m_box.getSize().y));
		return *this;
	}

	TextField& TextField::clearAfterInputIsProcessed(const bool shoudlClear)
	{
		m_clearAfterInputProcessed = shoudlClear;
		return *this;
	}

	TextField& TextField::setActive(const bool active)
	{
		m_active = active;
		return *this;
	}
	
	void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_prompt && m_input.getString().isEmpty())
			target.draw(*m_prompt, states);
		
		if (Duration(gui::Internals::timeSinceStart() - m_timeOfLastCursorFlick).count() > 0.5f)
		{
			m_cursorVisible = !m_cursorVisible;
			m_timeOfLastCursorFlick = gui::Internals::timeSinceStart();
		}

		sf::FloatRect rect(m_position, m_box.getSize());
		states.transform.transformRect(rect);
		m_box.setViewport(sf::FloatRect(rect.left / target.getSize().x, rect.top / target.getSize().y, rect.width / target.getSize().x, rect.height / target.getSize().y));

		const sf::View buffer(target.getView());
		target.setView(m_box);
		target.draw(m_input);
		target.setView(buffer);

		if (m_active && m_cursorVisible)
		{
			states.transform.translate(int(m_position.x) + int(getHeight() / 4), int(m_position.y));
			target.draw(m_cursor, states);
		}
	}

	void TextField::removeCharacter(const bool backspace)
	{
		if (m_input.getString().isEmpty() || m_cursorPosition == (backspace ? 0 : m_input.getString().getSize())) return;

		m_input.setString(m_input.getString().substring(0, m_cursorPosition - backspace) + m_input.getString().substring(m_cursorPosition + !backspace));
		setCursorPosition(m_cursorPosition - backspace);
	}

	void TextField::addCharacter(const sf::Uint32 character)
	{
		m_input.setString(operator+(m_input.getString().substring(0, m_cursorPosition), character) + m_input.getString().substring(m_cursorPosition));
		setCursorPosition(m_cursorPosition + 1);
	}

	void TextField::getClickedCharacter(const float x, const float y)
	{
		for (m_cursorPosition = 0; m_cursorPosition <= m_input.getString().getSize() &&
			x > (m_input.findCharacterPos(m_cursorPosition).x + (m_cursorPosition == 0 ? m_input.findCharacterPos(m_cursorPosition).x :
				m_input.findCharacterPos(m_cursorPosition - 1).x)) / 2 + m_position.x;
			++m_cursorPosition);
		setCursorPosition(m_cursorPosition != 0 ? m_cursorPosition - 1 : m_cursorPosition);
	}
}
