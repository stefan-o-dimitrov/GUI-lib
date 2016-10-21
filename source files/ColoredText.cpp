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

#include "../include/GUI/ColoredText.h"

#include <sstream>
#include <algorithm>

namespace gui
{
	ColoredText::ColoredText(const std::function<ColoredText()>& function)
	{
		m_volatileText.insert(std::move(std::make_pair(0, function)));
	}

	ColoredText::ColoredText(std::function<ColoredText()>&& function)
	{
		m_volatileText.insert(std::move(std::make_pair(0, std::move(function))));
	}

	gui::ColoredText::operator ColoredString()
	{
		return ColoredString(m_text.empty() ? "" : m_text.front()->first,
			m_text.empty() ? std::make_pair(sf::Color(), sf::Text::Regular) : m_text.front()->second);
	}

	void ColoredText::stringToArrangedText(unique_ptr_vector<sf::Text>& target,
		const ColoredString& str, const sf::Font& font,
		const unsigned char characterSize, sf::Vector2f& addPosition,
		const float TEXT_HEIGHT)
	{
		sf::String buffer;
		for (auto it = str.first.begin(), end = str.first.end(); it != end; ++it)
		{
			if (*it != '\n') buffer += *it;
			else
			{
				target.emplace_back(new sf::Text(buffer, font, characterSize));
				target.back()->setOutlineColor(sf::Color(15, 15, 15, 230));
				target.back()->setFillColor(str.second.first);
				target.back()->setStyle(str.second.second);
				target.back()->setPosition(int(addPosition.x), int(addPosition.y));
				addPosition.x = 0;
				addPosition.y += TEXT_HEIGHT + LINE_SPACING;
				buffer.clear();
			}
		}

		if (!buffer.isEmpty())
		{
			target.emplace_back(new sf::Text(buffer, font, characterSize));
			target.back()->setOutlineColor(sf::Color(15, 15, 15, 230));
			target.back()->setFillColor(str.second.first);
			target.back()->setStyle(str.second.second);
			target.back()->setPosition(int(addPosition.x), int(addPosition.y));
			addPosition.x += target.back()->getGlobalBounds().width + 1.0f;
		}
	}

	const bool ColoredText::isVolatile() const
	{
		return m_volatileText.size();
	}

	void ColoredText::getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize)const
	{
		const float TEXT_HEIGHT(font.getLineSpacing(characterSize) + LINE_SPACING);
		sf::Vector2f addPosition(0, 0);
		target.clear();

		getText(target, font, characterSize, TEXT_HEIGHT, addPosition);
	}

	void ColoredText::getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize, const float TEXT_HEIGHT, sf::Vector2f& addPosition)const
	{
		if (!m_text.empty())
		{
			for (unsigned short it = 0, end = m_text.size(); it < end; it++)
			{
				for (auto it1 = m_volatileText.equal_range(it); it1.first != it1.second; ++it1.first)
					it1.first->second().getText(target, font, characterSize, TEXT_HEIGHT, addPosition);
				stringToArrangedText(target, *m_text.at(it), font, characterSize, addPosition, TEXT_HEIGHT);
			}
			for (auto it = m_volatileText.equal_range(m_text.size()); it.first != it.second; ++it.first)
				it.first->second().getText(target, font, characterSize, TEXT_HEIGHT, addPosition);
		}
		else if (!m_volatileText.empty())
			for (auto it = m_volatileText.begin(), end = m_volatileText.end(); it != end; ++it)
				it->second().getText(target, font, characterSize, TEXT_HEIGHT, addPosition);

	}

	void ColoredText::arrangeText(unique_ptr_vector<sf::Text>& target)
	{
		if (!target.empty())
		{
			target.front()->setPosition(0, 0);
			for (auto it = target.begin() + 1, end = target.end(); it != end; ++it)
				(*it)->setPosition(int((*it)->getPosition().y > (*(it - 1))->getPosition().y ? 0 : (*(it - 1))->getPosition().x + (*(it - 1))->getGlobalBounds().width),
					int((*it)->getPosition().y > (*(it - 1))->getPosition().y ?
					(*(it - 1))->getPosition().y + (*(it - 1))->getGlobalBounds().height + LINE_SPACING : (*(it - 1))->getPosition().y));
		}
	}

	ColoredText bind(const sf::String& string, const sf::Color& color, const sf::Text::Style style)
	{
		ColoredText returnValue;
		if (!string.isEmpty()) returnValue.m_text.emplace_back(new ColoredString(string, std::make_pair(color, style)));
		return returnValue;
	}

	ColoredText bind(sf::String&& string, const sf::Color& color, const sf::Text::Style style)
	{
		ColoredText returnValue;
		if (!string.isEmpty()) returnValue.m_text.emplace_back(new ColoredString(std::move(string), std::make_pair(color, style)));
		return returnValue;
	}

	ColoredText operator+(const ColoredText& lhs, const ColoredText& rhs)
	{
		ColoredText returnValue(lhs);
		for (auto it = rhs.m_volatileText.begin(), end = rhs.m_volatileText.end(); it != end; ++it)
			returnValue.m_volatileText.emplace(std::move(std::make_pair(it->first + returnValue.m_text.size(), it->second)));
		for (auto it = rhs.m_text.begin(), end = rhs.m_text.end(); it != end; ++it)
			returnValue.m_text.emplace_back(*it);
		return returnValue;
	}

	ColoredText operator+(const ColoredText& lhs, ColoredText&& rhs)
	{
		ColoredText returnValue(lhs);
		for (auto it = rhs.m_volatileText.begin(), end = rhs.m_volatileText.end(); it != end; ++it)
			returnValue.m_volatileText.emplace(std::move(std::make_pair(it->first + returnValue.m_text.size(), std::move(it->second))));
		for (auto it = rhs.m_text.begin(), end = rhs.m_text.end(); it != end; ++it)
			returnValue.m_text.emplace_back(std::move(*it));
		return returnValue;
	}

	ColoredText operator+(const ColoredText& lhs, const ColoredString& rhs)
	{
		ColoredText returnValue(lhs);
		returnValue.m_text.emplace_back(new ColoredString(rhs));
		return returnValue;
	}

	ColoredText operator+(const ColoredText& lhs, ColoredString&& rhs)
	{
		ColoredText returnValue(lhs);
		returnValue.m_text.emplace_back(new ColoredString(std::move(rhs)));
		return returnValue;
	}

	ColoredText operator+(const ColoredText& lhs, const std::function<ColoredText()>& rhs)
	{
		ColoredText returnValue(lhs);
		returnValue.m_volatileText.insert(std::make_pair(returnValue.m_text.size(), rhs));
		return returnValue;
	}

	ColoredText operator+(const ColoredText& lhs, std::function<ColoredText()>&& function)
	{
		ColoredText returnValue(lhs);
		returnValue.m_volatileText.emplace(std::move(std::make_pair(returnValue.m_text.size(), std::move(function))));
		return returnValue;
	}


	ColoredText operator+(ColoredText&& lhs, const ColoredText& rhs)
	{
		ColoredText returnValue(std::move(lhs));
		for (auto it = rhs.m_volatileText.begin(), end = rhs.m_volatileText.end(); it != end; ++it)
			returnValue.m_volatileText.emplace(std::move(std::make_pair(it->first + returnValue.m_text.size(), it->second)));
		for (auto it = rhs.m_text.begin(), end = rhs.m_text.end(); it != end; ++it)
			returnValue.m_text.emplace_back(*it);
		return returnValue;
	}

	ColoredText operator+(ColoredText&& lhs, ColoredText&& rhs)
	{
		ColoredText returnValue(std::move(lhs));
		for (auto it = rhs.m_volatileText.begin(), end = rhs.m_volatileText.end(); it != end; ++it)
			returnValue.m_volatileText.emplace(std::move(std::make_pair(it->first + returnValue.m_text.size(), std::move(it->second))));
		for (auto it = rhs.m_text.begin(), end = rhs.m_text.end(); it != end; ++it)
			returnValue.m_text.emplace_back(std::move(*it));
		return returnValue;
	}

	ColoredText operator+(ColoredText&& lhs, const ColoredString& rhs)
	{
		ColoredText returnValue(std::move(lhs));
		returnValue.m_text.emplace_back(new ColoredString(rhs));
		return returnValue;
	}

	ColoredText operator+(ColoredText&& lhs, ColoredString&& rhs)
	{
		ColoredText returnValue(std::move(lhs));
		returnValue.m_text.emplace_back(new ColoredString(std::move(rhs)));
		return returnValue;
	}

	ColoredText operator+(ColoredText&& lhs, const std::function<ColoredText()>& rhs)
	{
		ColoredText returnValue(std::move(lhs));
		returnValue.m_volatileText.insert(std::make_pair(returnValue.m_text.size(), rhs));
		return returnValue;
	}

	ColoredText operator+(ColoredText&& lhs, std::function<ColoredText()>&& function)
	{
		ColoredText returnValue(std::move(lhs));
		returnValue.m_volatileText.emplace(std::move(std::make_pair(returnValue.m_text.size(), std::move(function))));
		return returnValue;
	}

	ColoredText operator+(const std::function<ColoredText()>& func1, const std::function<ColoredText()>& func2)
	{
		ColoredText returnValue;
		returnValue.m_volatileText.emplace(std::move(std::make_pair(0, func1)));
		returnValue.m_volatileText.emplace(std::move(std::make_pair(0, func2)));
		return returnValue;
	}

	ColoredText operator+(std::function<ColoredText()>&& func1, ColoredText&& string)
	{
		ColoredText returnValue;
		returnValue.m_volatileText.emplace(std::move(std::make_pair(0, std::move(func1))));
		return std::move(returnValue + std::move(string));
	}

	ColoredText operator+(std::function<ColoredText()>&& func1, const ColoredText& string)
	{
		ColoredText returnValue;
		returnValue.m_volatileText.emplace(std::move(std::make_pair(0, std::move(func1))));
		return std::move(returnValue + string);
	}

	ColoredText operator+(const std::function<ColoredText()>& func1, ColoredText&& string)
	{
		ColoredText returnValue;
		returnValue.m_volatileText.emplace(std::move(std::make_pair(0, func1)));
		return std::move(returnValue + std::move(string));
	}

	ColoredText operator+(const std::function<ColoredText()>& func1, const ColoredText& string)
	{
		ColoredText returnValue;
		returnValue.m_volatileText.emplace(std::move(std::make_pair(0, func1)));
		return std::move(returnValue + string);
	}
}