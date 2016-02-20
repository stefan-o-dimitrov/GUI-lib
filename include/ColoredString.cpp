#include <sstream>
#include <algorithm>

#include "ColoredString.h"

const float gui::ColoredString::LINE_SPACING = 5.0f;

void stringToArrangedText(gui::unique_ptr_vector<sf::Text>& target, const gui::cString& str, const sf::Font& font, const unsigned char characterSize, sf::Vector2f& addPosition, const float TEXT_HEIGHT)
{
	std::string lines(str.first);

	while (lines.size() > 0)
	{
		std::string line;

		const size_t pos = lines.find_first_of('\n');
		if (pos != std::string::npos)
		{
			line = lines.substr(0, pos);
			lines = lines.substr(pos + 1);
			addPosition.x = 0;
			addPosition.y += TEXT_HEIGHT;
		}
		else
		{
			line = lines;
			lines.clear();
		}
		target.emplace_back(new sf::Text(line, font, characterSize));
		target.back()->setPosition(addPosition);
		target.back()->setColor(str.second);

		addPosition.x += target.back()->getGlobalBounds().width + 1.0f;
	}
}

const bool gui::ColoredString::isVolatile() const
{
	return volatileText.size();
}

void gui::ColoredString::getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize)const
{
	const float TEXT_HEIGHT = sf::Text("I", font, characterSize).getGlobalBounds().height + LINE_SPACING;
	sf::Vector2f addPosition(0, 0);
	target.clear();

	getText(target, font, characterSize, TEXT_HEIGHT, addPosition);
}

void gui::ColoredString::getText(unique_ptr_vector<sf::Text>& target, const sf::Font& font, const unsigned char characterSize, const float TEXT_HEIGHT, sf::Vector2f& addPosition)const
{
	if(!text.empty())
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

void gui::ColoredString::arrangeText(unique_ptr_vector<sf::Text>& target)
{
	if (!target.empty())
	{
		target.front()->setPosition(0, 0);
		for (auto it = target.begin() + 1, end = target.end(); it != end; ++it)
			(*it)->setPosition( (*it)->getPosition().y > (*(it - 1))->getPosition().y ? 0 : (*(it - 1))->getPosition().x + (*(it - 1))->getGlobalBounds().width ,
				(*it)->getPosition().y > (*(it - 1))->getPosition().y ?
				(*(it - 1))->getPosition().y + (*(it - 1))->getGlobalBounds().height + LINE_SPACING : (*(it - 1))->getPosition().y);
	}
}

gui::ColoredString gui::bind(const std::string& string, const sf::Color& color)
{
	ColoredString returnValue;
	if(!string.empty()) returnValue.text.emplace_back(new cString(string, color));
	return returnValue;
}

gui::ColoredString& gui::operator+(ColoredString& lhs, const ColoredString& rhs)
{
	for (auto it = rhs.volatileText.begin(), end = rhs.volatileText.end(); it != end; ++it)
		lhs.volatileText.emplace(std::move(std::make_pair(it->first + lhs.text.size(), it->second)));
	for (auto it = rhs.text.begin(), end = rhs.text.end(); it != end; ++it)
		lhs.text.emplace_back(*it);
	return lhs;
}

gui::ColoredString& gui::operator+(ColoredString& lhs, ColoredString&& rhs)
{
	for (auto it = rhs.volatileText.begin(), end = rhs.volatileText.end(); it != end; ++it)
		lhs.volatileText.emplace(std::move(std::make_pair(it->first + lhs.text.size(), std::move(it->second))));
	for (auto it = rhs.text.begin(), end = rhs.text.end(); it != end; ++it)
		lhs.text.emplace_back(std::move(*it));
	return lhs;
}

gui::ColoredString& gui::operator+(ColoredString& lhs, const std::function<ColoredString()>& rhs)
{
	lhs.volatileText.insert(std::make_pair(lhs.text.size(), rhs));
	return lhs;
}

gui::ColoredString& gui::operator+(ColoredString& lhs, std::function<ColoredString()>&& rhs)
{
	lhs.volatileText.emplace(std::move(std::make_pair(lhs.text.size(), std::move(rhs))));
	return lhs;
}
