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
	const float ColoredText::LINE_SPACING = 5.0f;

	void ColoredText::stringToArrangedText(unique_ptr_vector<sf::Text>& target,
		const ColoredString& str, const sf::Font& font,
		const unsigned char characterSize, sf::Vector2f& addPosition,
		const float TEXT_HEIGHT)
	{
		std::string buffer;
		for (auto it = str.first.begin(), end = str.first.end(); it != end; ++it)
		{
			if (*it != '\n') buffer.push_back(*it);
			else
			{
				target.emplace_back(new sf::Text(buffer, font, characterSize));
				target.back()->setColor(str.second);
				target.back()->setPosition(addPosition);
				addPosition.x = 0;
				addPosition.y += TEXT_HEIGHT + LINE_SPACING;
				buffer.clear();
			}
		}

		if (!buffer.empty())
		{
			target.emplace_back(new sf::Text(buffer, font, characterSize));
			target.back()->setColor(str.second);
			target.back()->setPosition(addPosition);
			addPosition.x += target.back()->getGlobalBounds().width + 1.0f;
		}
	}

	const bool ColoredText::isVolatile() const
	{
		return volatileText.size();
	}

	void ColoredText::getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize)const
	{
		const float TEXT_HEIGHT = font.getLineSpacing(characterSize) + LINE_SPACING;
		sf::Vector2f addPosition(0, 0);
		target.clear();

		getText(target, font, characterSize, TEXT_HEIGHT, addPosition);
	}

	void ColoredText::getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize, const float TEXT_HEIGHT, sf::Vector2f& addPosition)const
	{
		if (!text.empty())
			for (unsigned short it = 0, end = text.size(); it < end; it++)
			{
				for (auto it1 = volatileText.equal_range(it); it1.first != it1.second; ++it1.first)
					it1.first->second().getText(target, font, characterSize, TEXT_HEIGHT, addPosition);
				stringToArrangedText(target, *text.at(it), font, characterSize, addPosition, TEXT_HEIGHT);
			}
		else if (!volatileText.empty())
			for (auto it = volatileText.begin(), end = volatileText.end(); it != end; ++it)
				it->second().getText(target, font, characterSize, TEXT_HEIGHT, addPosition);
	}

	void ColoredText::arrangeText(unique_ptr_vector<sf::Text>& target)
	{
		if (!target.empty())
		{
			target.front()->setPosition(0, 0);
			for (auto it = target.begin() + 1, end = target.end(); it != end; ++it)
				(*it)->setPosition((*it)->getPosition().y > (*(it - 1))->getPosition().y ? 0 : (*(it - 1))->getPosition().x + (*(it - 1))->getGlobalBounds().width,
					(*it)->getPosition().y > (*(it - 1))->getPosition().y ?
					(*(it - 1))->getPosition().y + (*(it - 1))->getGlobalBounds().height + LINE_SPACING : (*(it - 1))->getPosition().y);
		}
	}

	ColoredText bind(const std::string& string, const sf::Color& color)
	{
		ColoredText returnValue;
		if (!string.empty()) returnValue.text.emplace_back(new ColoredString(string, color));
		return returnValue;
	}

	ColoredText& operator+(ColoredText& lhs, const ColoredText& rhs)
	{
		for (auto it = rhs.volatileText.begin(), end = rhs.volatileText.end(); it != end; ++it)
			lhs.volatileText.emplace(std::move(std::make_pair(it->first + lhs.text.size(), it->second)));
		for (auto it = rhs.text.begin(), end = rhs.text.end(); it != end; ++it)
			lhs.text.emplace_back(*it);
		return lhs;
	}

	ColoredText& operator+(ColoredText& lhs, ColoredText&& rhs)
	{
		for (auto it = rhs.volatileText.begin(), end = rhs.volatileText.end(); it != end; ++it)
			lhs.volatileText.emplace(std::move(std::make_pair(it->first + lhs.text.size(), std::move(it->second))));
		for (auto it = rhs.text.begin(), end = rhs.text.end(); it != end; ++it)
			lhs.text.emplace_back(std::move(*it));
		return lhs;
	}

	ColoredText& operator+(ColoredText& lhs, const ColoredString& rhs)
	{
		lhs.text.emplace_back(new ColoredString(rhs));
		return lhs;
	}

	ColoredText& operator+(ColoredText& lhs, ColoredString&& rhs)
	{
		lhs.text.emplace_back(new ColoredString(std::move(rhs)));
		return lhs;
	}

	ColoredText& operator+(ColoredText& lhs, const std::function<ColoredText()>& rhs)
	{
		lhs.volatileText.insert(std::make_pair(lhs.text.size(), rhs));
		return lhs;
	}

	ColoredText& operator+(ColoredText& lhs, std::function<ColoredText()>&& rhs)
	{
		lhs.volatileText.emplace(std::move(std::make_pair(lhs.text.size(), std::move(rhs))));
		return lhs;
	}
}