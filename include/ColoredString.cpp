#include <sstream>

#include "ColoredString.h"

gui::ColoredString::ColoredString() {}

gui::ColoredString::ColoredString(const ColoredString& _lVal)
{
	for (auto it = _lVal.vec.begin(), end = _lVal.vec.end(); it != end; ++it)
		vec.emplace_back(new cString(*(*it)));
}

gui::ColoredString& gui::ColoredString::operator=(const ColoredString& _lVal)
{
	for (auto it = _lVal.vec.begin(), end = _lVal.vec.end(); it != end; ++it)
		vec.emplace_back(new cString(*(*it)));
	return *this;
}

std::vector<std::unique_ptr<sf::Text>> gui::ColoredString::interpret(const ColoredString& string, const sf::Font& font, const unsigned char characterSize)
{
	std::vector<std::unique_ptr<sf::Text>> returnValue;
	if (string.vec.empty()) return returnValue;

	const float TEXT_HEIGHT = sf::Text("|", font, characterSize).getLocalBounds().height;
	sf::Vector2f addPosition(0, 0);

	for (auto it = string.vec.begin(), end = string.vec.end(); it != end; ++it)
	{
		std::string lines((*it)->first);

		while (lines.size() != 0)
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
			returnValue.back()->setColor((*it)->second);

			addPosition.x += returnValue.back()->getGlobalBounds().width;
		}
	}
	return returnValue;
}

gui::ColoredString gui::bind(const std::string& string, const sf::Color& color)
{
	ColoredString returnValue;
	returnValue.vec.emplace_back(new cString(string, color));
	return returnValue;
}

gui::ColoredString gui::operator+(ColoredString& lhs, ColoredString&& rhs)
{
	for (auto it = rhs.vec.begin(), end = rhs.vec.end(); it != end; ++it)
		lhs.vec.emplace_back(std::move(*it));
	return lhs;
}