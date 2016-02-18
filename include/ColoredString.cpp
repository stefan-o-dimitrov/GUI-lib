#include <sstream>
#include <algorithm>

#include "ColoredString.h"

const float gui::ColoredString::LINE_SPACING = 5.0f;

gui::ColoredString::ColoredString(const ColoredString& _lVal)
{
	for (auto it = _lVal.text.begin(), end = _lVal.text.end(); it != end; ++it)
		text.emplace_back(new cString(*(*it)));

	for (auto it = _lVal.volatileText.begin(), end = _lVal.volatileText.end(); it != end; ++it)
		volatileText.emplace(*it);
}

gui::ColoredString& gui::ColoredString::operator=(const ColoredString& copy)
{
	for (auto it = copy.text.begin(), end = copy.text.end(); it != end; ++it)
		text.emplace_back(new cString(*(*it)));

	for (auto it = copy.volatileText.begin(), end = copy.volatileText.end(); it != end; ++it)
		volatileText.emplace(*it);

	return *this;
}

void addText(const std::string& str, sf::Vector2f& addPosition, const float TEXT_HEIGHT, gui::ptr_vector<sf::Text>& returnValue, const sf::Font& font, const unsigned char characterSize, const sf::Color& color)
{
	std::string lines(str);

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
		returnValue.emplace_back(new sf::Text(line, font, characterSize));
		returnValue.back()->setPosition(addPosition);
		returnValue.back()->setColor(color);

		addPosition.x += returnValue.back()->getGlobalBounds().width;
	}
}

gui::ptr_vector<sf::Text> gui::ColoredString::interpret(const ColoredString& string, const sf::Font& font, const unsigned char characterSize)
{
	ptr_vector<sf::Text> returnValue;
	if (string.text.empty() && string.volatileText.empty()) return returnValue;
	
	const float TEXT_HEIGHT = sf::Text("I", font, characterSize).getGlobalBounds().height + LINE_SPACING;
	sf::Vector2f addPosition(0, 0);

	if(!string.text.empty())
		for (unsigned short it = 0, end = string.text.size(); it < end; it++)
		{
			for (auto it1 = string.volatileText.equal_range(it); it1.first != it1.second; ++it1.first)
			{
				const ColoredString buffer(std::move(it1.first->second()));
				for (auto it2 = buffer.text.begin(), end2 = buffer.text.end(); it2 != end2; ++it2)
					addText((*it2)->first, addPosition, TEXT_HEIGHT, returnValue, font, characterSize, (*it2)->second);
			}

			const auto buffer = *string.text.at(it);

			addText(buffer.first, addPosition, TEXT_HEIGHT, returnValue, font, characterSize, buffer.second);
		
		}
	else
		for (auto it = string.volatileText.begin(), end = string.volatileText.end(); it != end; ++it)
		{
			const ColoredString buffer(std::move(it->second()));
			for (auto it2 = buffer.text.begin(), end2 = buffer.text.end(); it2 != end2; ++it2)
				addText((*it2)->first, addPosition, TEXT_HEIGHT, returnValue, font, characterSize, (*it2)->second);
		}
	return returnValue;
}

gui::ptr_vector<sf::Text> gui::ColoredString::rearrangeText(const ptr_vector<sf::Text>& string, const sf::Font& font, const unsigned char characterSize)
{
	ptr_vector<sf::Text> returnValue;
	if (string.empty()) return returnValue;

	const float TEXT_HEIGHT = sf::Text("I", font, characterSize).getGlobalBounds().height + LINE_SPACING;
	sf::Vector2f addPosition(0, 0);

	for (auto it = string.begin(), end = string.end(); it != end; ++it)
	{
		std::string lines((*it)->getString());

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
			returnValue.emplace_back(new sf::Text(line, font, characterSize));
			returnValue.back()->setPosition(addPosition);
			returnValue.back()->setColor((*it)->getColor());

			addPosition.x += returnValue.back()->getGlobalBounds().width;
		}
	}
	return returnValue;
}

gui::ColoredString gui::bind(const std::string& string, const sf::Color& color)
{
	ColoredString returnValue;
	returnValue.text.emplace_back(new cString(string, color));
	return returnValue;
}

gui::ColoredString& gui::operator+(ColoredString& lhs, ColoredString&& rhs)
{
	for (auto it = rhs.text.begin(), end = rhs.text.end(); it != end; ++it)
		lhs.text.emplace_back(std::move(*it));
	for (auto it = rhs.volatileText.begin(), end = rhs.volatileText.end(); it != end; ++it)
		lhs.volatileText.emplace(std::move(*it));
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
