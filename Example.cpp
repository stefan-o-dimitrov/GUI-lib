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
	
	gui::Window main;
	
	main.add(std::move(gui::Button(
		gui::Icon(buttonTex, true),
		std::bind(increment, -AMOUNT))
		.setPredicates(gui::Button::predicateArray { std::make_pair(std::bind(canChange, -LIMIT, true), "Integer less than 10.") },
			font)
		.setName(std::move(gui::TextArea("Int-=" + std::to_string(AMOUNT), font, 18).setColor(sf::Color::Green)))
		.setDelay(0.5f)
		.setMessage(std::move(gui::HoverMessage(
			gui::bind("This button ", sf::Color::White) + gui::bind("reduces ", sf::Color::Red) + gui::bind("the integer.", sf::Color::White),
			font)
			.setBackgroundFill(sf::Color::Black)
			.setBorderFill(sf::Color::Blue)
			.setBorderThickness(2.0f)
			.setCharacterSize(15)))
		.setPosition(110, 200)))

	.add(gui::Button(
		gui::Icon(buttonTex, false),
		std::bind(increment, AMOUNT))
		.setPredicates(gui::Button::predicateArray{ std::make_pair(std::bind(canChange, LIMIT, false), "Integer greater than 10.") },
			font)
		.setName(gui::TextArea("Int+=" + std::to_string(AMOUNT), font, 18).setColor(sf::Color::Yellow))
		.setDelay(0.5f)
		.setMessage(gui::HoverMessage(
			gui::bind("This button ", sf::Color::White) + gui::bind("increases ", sf::Color::Green) + gui::bind("the integer.", sf::Color::White),
			font)
			.setBackgroundFill(sf::Color::Black)
			.setBorderFill(sf::Color::Blue)
			.setBorderThickness(2.0f)
			.setCharacterSize(15))
		.setPosition(700, 200))

	.add(std::move(gui::TextArea("", font, 40)
		.setColor(sf::Color::Red)
		.setUpdateFunction(getInt)
		.setMessage(std::move(gui::HoverMessage(
			gui::bind("This is the ", sf::Color::White) + gui::bind("current value ", sf::Color::Yellow) + gui::bind("of the ", sf::Color::White) + gui::bind("integer.", sf::Color::Yellow) +
			gui::bind("\nThis text gets updated automatically every 1 / 25 second.", sf::Color::Red), font, 15)
			.setBackgroundFill(sf::Color::Black)
			.setBorderFill(sf::Color::Yellow)
			.setBorderThickness(2.0f)))
		.setPosition(100, 100)));

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

			main.input(event);
		}

		window.draw(main);

		window.display();
		window.clear(sf::Color(190, 190, 190, 255));
	}
}