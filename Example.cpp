#include "include/GUI.h"
#include <windows.h>
#include <sstream>
#include <iostream>
#define LIMIT 10
#define AMOUNT 1

int integer = 0;

void increment(const int amount)
{
	integer += amount;
}

const bool canChange(const int limit, const bool increaseOrDecrease = 0)
{
	return increaseOrDecrease ? integer > limit : integer < limit;
}

std::string getInt()
{
	std::stringstream ss;
	ss << integer;
	return ss.str();
}

void main()
{
	sf::Texture buttonTex;
	sf::Font font;

	buttonTex.loadFromFile("button.png");
	font.loadFromFile("font.ttf");
	
	std::vector<std::unique_ptr<gui::Interactive>> elements;
	
	elements.emplace_back(new gui::Button(dynamic_cast<gui::Button&&>(gui::Button(
		gui::Icon(buttonTex, true),
		std::bind(increment, -AMOUNT))
		.setPredicates(gui::Button::predicateArray { std::make_pair(std::bind(canChange, -LIMIT, true), "Integer less than 10.") },
			font)
		.setDelay(0.5f)
		.setMessage(gui::HoverMessage(
			gui::bind("This button ", sf::Color::White) + gui::bind("reduces ", sf::Color::Red) + gui::bind("the integer.", sf::Color::White),
			font)
			.setBackgroundFill(sf::Color::Black)
			.setBorderFill(sf::Color::Blue)
			.setBorderThickness(2.0f)
			.setCharacterSize(15))
		.setPosition(110, 200)
		)));
	
	elements.emplace_back(new gui::Button(dynamic_cast<gui::Button&&>(gui::Button(
		gui::Icon(buttonTex, true),
		std::bind(increment, AMOUNT))
		.setPredicates(gui::Button::predicateArray{ std::make_pair(std::bind(canChange, LIMIT, false), "Integer greater than 10.") },
			font)
		.setDelay(0.5f)
		.setMessage(gui::HoverMessage(
			gui::bind("This button ", sf::Color::White) + gui::bind("increases ", sf::Color::Green) + gui::bind("the integer.", sf::Color::White),
			font)
			.setBackgroundFill(sf::Color::Black)
			.setBorderFill(sf::Color::Blue)
			.setBorderThickness(2.0f)
			.setCharacterSize(15))
		.setPosition(700, 200)
		)));

	elements.emplace_back(new gui::TextArea(dynamic_cast<gui::TextArea&&>(
		gui::TextArea("", font, 40)
		.setColor(sf::Color::Red)
		.setUpdateFunction(getInt)
		.setMessage(gui::HoverMessage(
			gui::bind("This is the current value of the integer.", sf::Color::White) +
			gui::bind("\nThis text gets updated automatically every 1 / 25 second.", sf::Color::Red), font, 15)
			.setBackgroundFill(sf::Color::Black)
			.setBorderFill(sf::Color::Yellow)
			.setBorderThickness(2.0f))
		.setPosition(100, 100))
		));

	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Example", sf::Style::Fullscreen);

	while (true)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return;
			}

			for (auto it = elements.rbegin(), end = elements.rend(); it != end; ++it)
				if ((*it)->input(event))
					break;
		}

		for (auto it = elements.begin(), end = elements.end(); it != end; ++it)
			window.draw(**it);

		window.display();
		window.clear(sf::Color(190, 190, 190, 255));
	}
}