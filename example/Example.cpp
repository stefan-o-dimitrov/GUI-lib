#include "../include/GUI.h"

#include <windows.h>
#include <sstream>

#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define LIMIT 10
#define AMOUNT 1

int integer = 0;
bool running = true;

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
	sf::Texture buttonTex, closeButtonTex, barBackgroundTex, barFillTex;
	sf::Font font;

	barBackgroundTex.loadFromFile("resources/bar_background.png");
	barFillTex.loadFromFile("resources/bar_fill.png");
	buttonTex.loadFromFile("resources/button.png");
	closeButtonTex.loadFromFile("resources/close_button.png");
	font.loadFromFile("resources/font.ttf");
	
	gui::Window main;
	sf::RenderWindow window(sf::VideoMode(800, 300), "Example", sf::Style::None);
	
	main.add(std::move(gui::Button(gui::Icon(buttonTex, true), std::bind([](const int amount) {integer += amount; }, -AMOUNT))
		.setPredicates(gui::PredicateArray{ std::bind(canChange, -LIMIT, true) })
		.setPredicateMessage(gui::HoverMessage(gui::bind("Integer is less than ", sf::Color::White) + 
			gui::bind(std::to_string(LIMIT), sf::Color::Yellow),
			font, 15))
		.setName(std::move(gui::TextArea("Decrease Int by " + std::to_string(AMOUNT), font, 18).setColor(sf::Color::Red)))
		.setDelay(0.5f)
		.setMessage((gui::HoverMessage&&)gui::HoverMessage(gui::bind("This button ", sf::Color::White) +
			gui::bind("reduces ", sf::Color::Red) + gui::bind("the integer", sf::Color::White) + []()
				{
					return integer == 0 ? gui::bind("", sf::Color()) : gui::bind(". It is ", sf::Color::White)
						+ gui::bind("\ncurrently at ", sf::Color::White) + gui::bind(std::to_string(integer), integer > 0.0f ?
							sf::Color::Green : sf::Color::Red);
				} + gui::bind(".", sf::Color::White),
			font)
			.setBackgroundFill(sf::Color::Black)
			.setBorderFill(sf::Color::Blue)
			.setBorderThickness(2.0f)
			.setCharacterSize(15))
		.setPosition(50, 150)))

		.add(gui::Button(gui::Icon(buttonTex, false), std::bind([](const int amount) {integer += amount; }, AMOUNT))
			.setPredicates(gui::PredicateArray{ std::bind(canChange, LIMIT, false) })
			.setName(gui::TextArea("Increase Int by " + std::to_string(AMOUNT), font, 18).setColor(sf::Color::Green))
			.setDelay(0.5f)
			.setMessage((gui::HoverMessage&&)gui::HoverMessage(gui::bind("This button ", sf::Color::White) +
				gui::bind("increases ", sf::Color::Green) + gui::bind("the integer.", sf::Color::White),
				font)
				.setBackgroundFill(sf::Color::Black)
				.setBorderFill(sf::Color::Blue)
				.setBorderThickness(2.0f)
				.setCharacterSize(15))
			.setPosition(50 + buttonTex.getSize().x, 150))

		.add(std::move(gui::TextArea("", font, 40)
			.setMessage(std::move(gui::HoverMessage(gui::bind("This is the ", sf::Color::White) + 
				gui::bind("current value ", sf::Color::Yellow) + gui::bind("of the ", sf::Color::White) + gui::bind("integer.", sf::Color::Yellow) +
				gui::bind("\nThis text gets updated automatically every 1 / 10 second.", sf::Color::Red), font, 15)
				.setBackgroundFill(sf::Color::Black)
				.setBorderFill(sf::Color::Yellow)
				.setBorderThickness(2.0f)))
			.setColor(sf::Color::Red)
			.setUpdateFunction([]()
				{
					return gui::ColoredString(std::to_string(integer), integer > 0.0f ? sf::Color::Green : integer == 0.0f ? 
						sf::Color::Yellow : sf::Color::Red);
				})			
			.setPosition(50, 50)))

		.add(gui::ProgressBar(gui::Icon(barBackgroundTex, true), gui::Icon(barFillTex, true))
			.setUpdateFunction(std::bind(getProgress, LIMIT))
			.setPosition(50, 175 + buttonTex.getSize().y)
			.setFillMessage(gui::HoverMessage(gui::bind("This message is superfluous", sf::Color::Yellow), font).setBackgroundFill(sf::Color::Black))
			.setMessage(gui::HoverMessage(gui::bind("This progress bar represents the integer's progress toward ", sf::Color::White)
				+ gui::bind(std::to_string(LIMIT), sf::Color::White), font).setBackgroundFill(sf::Color::Black)))

		.add(gui::ProgressBar(gui::Icon(barBackgroundTex, true), gui::Icon(barFillTex, true))
			.setUpdateFunction(std::bind(getProgress, -LIMIT))
			.setPosition(50, 200 + buttonTex.getSize().y + barBackgroundTex.getSize().y)
			.setMessage(gui::HoverMessage(gui::bind("This progress bar represents the integer's progress toward -", sf::Color::White)
				+ gui::bind(std::to_string(LIMIT), sf::Color::White), font).setBackgroundFill(sf::Color(20, 30, 40, 210))))
					
		.add(gui::Button(gui::Icon(closeButtonTex, true),
			[]() {running = false; })
			.setPosition(window.getSize().x - closeButtonTex.getSize().x, 0));
				
	window.setVerticalSyncEnabled(true);

	while (running)
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed) running = false;
			else main.input(event);

		window.draw(main);

		window.display();
		window.clear(sf::Color(190, 190, 190, 255));
	}
}