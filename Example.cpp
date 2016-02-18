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

const float getProgress(const int limit)
{
	return float(integer) / limit;
}

std::string getInt()
{
	std::stringstream ss;
	ss << integer;
	return ss.str();
}

void main()
{
	sf::Texture buttonTex, barBackgroundTex, barFillTex;
	sf::Font font;

	barBackgroundTex.loadFromFile("bar_background.png");
	barFillTex.loadFromFile("bar_fill.png");
	buttonTex.loadFromFile("button.png");
	font.loadFromFile("font.ttf");
	
	gui::Window main;
	
	main.add(std::move(gui::Button(
		gui::Icon(buttonTex, true),
		std::bind(increment, -AMOUNT))
		.setPredicates(gui::Button::predicateArray{ std::make_pair(std::bind(canChange, -LIMIT, true), "Integer less than 10.") },
			font)
		.setName(std::move(gui::TextArea("Decrease Int by " + std::to_string(AMOUNT), font, 18).setColor(sf::Color::Green)))
		.setDelay(0.5f)
		.setMessage(std::move(gui::HoverMessage(
			gui::bind("This button ", sf::Color::White) + gui::bind("reduces ", sf::Color::Red) + gui::bind("the integer. It is ", sf::Color::White)
			+ gui::bind("\ncurrently at ", sf::Color::White) + []()
				{
					return gui::bind(std::to_string(integer), integer > 0.0f ? sf::Color::Green : integer == 0.0f ? sf::Color::Yellow : sf::Color::Red);
				} + gui::bind(".", sf::Color::White),
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
			.setName(gui::TextArea("Increase Int by " + std::to_string(AMOUNT), font, 18).setColor(sf::Color::Yellow))
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
			.setUpdateFunction([]()
				{
					return gui::bind(std::to_string(integer), integer > 0.0f ? sf::Color::Green : integer == 0.0f ? sf::Color::Yellow : sf::Color::Red);
				})
			.setMessage(std::move(gui::HoverMessage(
				gui::bind("This is the ", sf::Color::White) + gui::bind("current value ", sf::Color::Yellow) + gui::bind("of the ", sf::Color::White) + gui::bind("integer.", sf::Color::Yellow) +
				gui::bind("\nThis text gets updated automatically every 1 / 10 second.", sf::Color::Red), font, 15)
				.setBackgroundFill(sf::Color::Black)
				.setBorderFill(sf::Color::Yellow)
				.setBorderThickness(2.0f)))
			.setPosition(100, 100)))

		.add(gui::ProgressBar(gui::Icon(barBackgroundTex, true), gui::Icon(barFillTex, true))
			.setUpdateFunction(std::bind(getProgress, LIMIT))
			.setPosition(100, 500)
			.setFillMessage(gui::HoverMessage(gui::bind("This message is superfluous", sf::Color::Yellow), font).setBackgroundFill(sf::Color::Black))
			.setMessage(gui::HoverMessage(gui::bind("This progress bar represents the integer's progress toward 10", sf::Color::White), font).setBackgroundFill(sf::Color::Black)))

		.add(gui::ProgressBar(gui::Icon(barBackgroundTex, true), gui::Icon(barFillTex, true))
			.setUpdateFunction(std::bind(getProgress, -LIMIT))
			.setPosition(100, 550)
			.setMessage(gui::HoverMessage(gui::bind("This progress bar represents the integer's progress toward -10", sf::Color::White), font).setBackgroundFill(sf::Color(20, 30, 40, 210))));


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