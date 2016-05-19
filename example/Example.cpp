#include <GUI\GUI.h>

#include <sstream> // Items needed only for this example start here.

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
} // Items needed only for this example end here.

void main()
{
	sf::Texture buttonTex, closeButtonTex, barBackgroundTex, barFillTex, windowBackground; // Loading resources begins here.
	sf::Font font;

	barBackgroundTex.loadFromFile("resources/bar_background.png");
	barFillTex.loadFromFile("resources/bar_fill.png");
	buttonTex.loadFromFile("resources/button.png");
	closeButtonTex.loadFromFile("resources/close_button.png");
	windowBackground.loadFromFile("resources/window_background.png");
	font.loadFromFile("resources/font.ttf"); // Loading resources ends here.

	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Example", sf::Style::None); // Creating Render Window.

	gui::WindowManager main; // Creating main Window Manager
	
	main.emplace("First Window", std::move(gui::Window() // Populating main with windows.
			.setPosition(250, 250)
			.setBackgroundTexture(windowBackground, true)
			.setMovable(true)

			.add("Txt Field", gui::TextField().setPosition(50, 130).setPrompt("Click here to type.").setFont(font).setColor(sf::Color::Red).setCharacterSize(13).setWidth(100))

			.add("1st Button", gui::Button(gui::Icon(buttonTex, false))
				.bindAction(gui::Button::Released, std::bind([](const int amount) 
					{
						integer += amount;
					}, AMOUNT))
				.setPredicates(gui::Button::PredicateArray{ std::bind(canChange, LIMIT, false) })
				.setPredicateMessage(gui::HoverMessage(gui::bind("Integer is greater than ", sf::Color::White) +
					gui::bind(std::to_string(LIMIT), sf::Color::Yellow),
					font, 15))
				.setName(gui::TextArea("Increase Int by " + std::to_string(AMOUNT), font, 18).setColor(sf::Color::Green))
				.setDelay(0.5f)
				.setMessage(std::move(gui::HoverMessage(gui::bind("This button ", sf::Color::White) +
					gui::bind("increases ", sf::Color::Green) + gui::bind("the integer.", sf::Color::White),
					font)
					.setBackgroundFill(sf::Color::Black)
					.setBorderFill(sf::Color::Blue)
					.setBorderThickness(-1.0f)
					.setBorderThickness(2.0f)
					.setCharacterSize(15)))
				.setPosition(50 + buttonTex.getSize().x, 130))

			.add("Txt Area", std::move(gui::TextArea("0", font, 40)
				.setMessage(std::move(gui::HoverMessage(gui::bind("This is the ", sf::Color::White) + 
					gui::bind("current value ", sf::Color::Yellow) + gui::bind("of the ", sf::Color::White) +
					gui::bind("integer.", sf::Color::Yellow) +
					gui::bind("\nThis text gets updated automatically every 1 / 10 seconds.", sf::Color::Red), font, 15)
					.setBackgroundFill(sf::Color::Black)
					.setBorderFill(sf::Color::Yellow)
					.setBorderThickness(2.0f)))
				.setColor(sf::Color::Yellow)
				.setUpdateFunction([]()
					{
						return gui::bind(std::to_string(integer), integer > 0.0f ? sf::Color::Green : integer == 0.0f ? 
							sf::Color::Yellow : sf::Color::Red);
					})
				.setPosition(50, 30)))

			.add("Int Progress", gui::ProgressBar(gui::Icon(barBackgroundTex, true), gui::Icon(barFillTex, true))
				.setUpdateFunction(std::bind(getProgress, LIMIT))
				.setPosition(50, 175 + buttonTex.getSize().y)
				.setFillMessage(gui::HoverMessage(gui::bind("This message is superfluous", sf::Color::Yellow), font)
					.setBackgroundFill(sf::Color::Black))
				.setMessage(gui::HoverMessage(gui::bind("This progress bar represents the integer's progress toward ", sf::Color::White)
					+ gui::bind(std::to_string(LIMIT), sf::Color::White), font).setBackgroundFill(sf::Color::Black)))

			.add("Int Progress 2", gui::ProgressBar(gui::Icon(barBackgroundTex, true), gui::Icon(barFillTex, true))
				.setUpdateFunction(std::bind(getProgress, -LIMIT))
				.setPosition(50, 200 + buttonTex.getSize().y + barBackgroundTex.getSize().y)
				.setMessage(gui::HoverMessage(gui::bind("This progress bar represents the integer's progress toward -", sf::Color::White)
					+ gui::bind(std::to_string(LIMIT), sf::Color::White), font).setBackgroundFill(sf::Color(20, 30, 40, 210))))
					
			.add("Close Btn", gui::Button(gui::Icon(closeButtonTex, true))
				.bindAction(gui::Button::Released, [&]()
					{
						main.at("First Window", false).setActive(false);
					})
				.setPosition(windowBackground.getSize().x - closeButtonTex.getSize().x, 0))
					
			.add("Txt Pane", gui::TextPane(gui::bind("This is a simple program demonstrating ", sf::Color::White) +
				gui::bind("\nSHT Games", sf::Color::Yellow, sf::Text::Italic) + 
				gui::bind("' GUI Library. If you encounter\nany issues, please contact us at:\n", sf::Color::White) +
				gui::bind("shtgamessts@gmail.com", sf::Color::Yellow, sf::Text::Bold), font, 15)
				.setPosition(120, 20)))
			.add("FPS", gui::FPSMeter().setFont(font).setPosition(600, 20).setColor(sf::Color::White)), false)
		
		.emplace("Second Window", std::move(gui::Window()
			.setBackgroundColor(sf::Color(255, 255, 150))
			.setBackgroundTexture(windowBackground)
			.setMovable(true)
			.setPosition(20, 20)
			.setBackgroundTextureRect(sf::IntRect(0, 0, 400, 200))

			.add("Txt Area", gui::TextArea()
				.setFont(font)
				.setText("0")
				.setUpdateFunction([]()
				{
					return gui::bind(std::to_string(integer), sf::Color());
				})
				.setPosition(30, 30)
				.setCharacterSize(30))

			.add("Btn", gui::Button(gui::Icon(buttonTex, true))
				.bindAction(gui::Button::Released, std::bind([](const int amount)
				{
					integer += amount;
				}, -AMOUNT))
				.bindAction(gui::Button::PredicatesUnfulfilled, [&]()
				{
					main.at("First Window", false).setActive(true);
				})
				.setPredicates(gui::Button::PredicateArray{ std::bind(canChange, -LIMIT, true) })
				.setPredicateMessage(gui::HoverMessage(gui::bind("Integer is less than ", sf::Color::White) + 
					gui::bind(std::to_string(LIMIT), sf::Color::Yellow),
					font, 15))
				.setName(std::move(gui::TextArea("int -= " + 
					std::to_string(AMOUNT), font, 18).setColor(sf::Color(175, 45, 65, 220))))
				.setPosition(30, 100))), true);

	main.emplace("Third Window", gui::Window(main.at("Second Window", true)).setBackgroundColor(sf::Color(255, 255, 200)).setPosition(45, 80), false);
	
	while (running)
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed) running = false;
			else main.input(event);

		window.draw(main);

		window.display();
		window.clear(sf::Color(150, 150, 150, 255));
	}
}
