#include <sstream>

#include "ColoredString.h"

gui::ColoredString::ColoredString() {}

gui::ColoredString::ColoredString(const ColoredString& _lVal)
{
	for (auto it = _lVal.vec.begin(), end = _lVal.vec.end(); it != end; ++it)
		vec.push_back(std::unique_ptr<cString>(new cString(*(*it))));
}

gui::ColoredString& gui::ColoredString::operator=(const ColoredString& _lVal)
{
	for (auto it = _lVal.vec.begin(), end = _lVal.vec.end(); it != end; ++it)
		vec.push_back(std::unique_ptr<cString>(new cString(*(*it))));
	return *this;
}

std::vector<std::unique_ptr<sf::Text>> gui::ColoredString::interpret(const ColoredString& string, const sf::Font& font, const unsigned char characterSize)
{
	std::vector<std::unique_ptr<sf::Text>> returnValue;
	if (string.vec.empty()) return returnValue;

	const unsigned char TEXT_HEIGHT = sf::Text("|", font, characterSize).getLocalBounds().height;
	sf::Vector2f addPosition(0, 0);
	unsigned short rectWidth = 0;

	for (auto it = string.vec.begin(), end = string.vec.end(); it != end; ++it)
	{
		std::stringstream lines((*it)->first);
		for (std::string line; std::getline(lines, line);)
		{
			returnValue.push_back(std::unique_ptr<sf::Text>(new sf::Text(line, font, characterSize)));
			returnValue.back()->setPosition(addPosition.x = 0, addPosition.y += TEXT_HEIGHT);
			returnValue.back()->setColor((*it)->second);

			float width = returnValue.back()->getGlobalBounds().width;
			addPosition.x += width;
			if (width > rectWidth) rectWidth = width;
		}
	}
	return returnValue;
}

gui::ColoredString gui::bind(const std::string& string, const sf::Color& color)
{
	ColoredString returnValue;
	returnValue.vec.push_back(std::unique_ptr<cString>(new cString(string, color)));
	return returnValue;
}

gui::ColoredString gui::operator+(ColoredString& lhs, ColoredString&& rhs)
{
	for (auto it = rhs.vec.begin(), end = rhs.vec.end(); it != end; ++it)
		lhs.vec.push_back(std::unique_ptr<cString>(std::move(*it)));
	return lhs;
}