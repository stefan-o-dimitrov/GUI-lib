#include <sstream>

#include "ColoredString.h"

const float gui::ColoredString::LINE_SPACING = 5.0f;

gui::ColoredString::ColoredString(const ColoredString& _lVal)
{
	for (auto it = _lVal.text.begin(), end = _lVal.text.end(); it != end; ++it)
		text.emplace_back(new cString(*(*it)));
}

std::vector<std::unique_ptr<sf::Text>> gui::ColoredString::interpret(const ColoredString& string, const sf::Font& font, const unsigned char characterSize)
{
	std::vector<std::unique_ptr<sf::Text>> returnValue;
	if (string.text.empty()) return returnValue;
	
	const float TEXT_HEIGHT = sf::Text("I", font, characterSize).getGlobalBounds().height + LINE_SPACING;
	sf::Vector2f addPosition(0, 0);

	for (auto it = string.text.begin(), end = string.text.end(); it != end; ++it)
	{
		std::string lines((*it)->first);

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
			returnValue.back()->setColor((*it)->second);

			addPosition.x += returnValue.back()->getGlobalBounds().width;
		}
	}
	return returnValue;
}

std::vector<std::unique_ptr<sf::Text>> gui::ColoredString::reinterpret(const std::vector<std::unique_ptr<sf::Text>>& string, const sf::Font & font, const unsigned char characterSize)
{
	std::vector<std::unique_ptr<sf::Text>> returnValue;
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
	return lhs;
}

gui::ColoredString& gui::operator+(ColoredString& lhs, const std::function<ColoredString()>& rhs)
{
	lhs.volatileText.push_back(std::make_pair(rhs, lhs.text.size()));
	return lhs;
}

gui::ColoredString& gui::operator+(ColoredString& lhs, std::function<ColoredString()>&& rhs)
{
	lhs.volatileText.push_back(std::move(std::make_pair(std::move(rhs), lhs.text.size())));
	return lhs;
}
