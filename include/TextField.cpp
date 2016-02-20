#include "TextField.h"

namespace gui
{
	TextField::TextField(const std::function<void(const std::string&)>& processTextInputFunction, const unsigned short fieldWidth, const sf::Font& font, const unsigned char characterSize)
		: processingFunction(processTextInputFunction), currentInput("", font, characterSize)
	{
		box.reset(sf::FloatRect(0, 0, fieldWidth, sf::Text("I", font, characterSize).getGlobalBounds().height));
	}

	TextField::TextField(const TextField& source)
		: box(source.box), currentInput(source.currentInput), processingFunction(source.processingFunction), allowClipboard(source.allowClipboard)
	{
		if (source.prompt) prompt.reset(new sf::Text(*source.prompt));
	}

	const bool TextField::input(const sf::Event& event)
	{
		switch (event.type)
		{
		case sf::Event::MouseMoved:
			break;
		case sf::Event::MouseButtonPressed:
			break;
		case sf::Event::MouseButtonReleased:
			break;
		case sf::Event::TextEntered:
			break;
		case sf::Event::KeyPressed:
			break;
		case sf::Event::LostFocus:
			break;
		default:
			return false;
		}
	}

	void TextField::processCurrentInput()
	{
		processingFunction(currentInput.getString());
		currentInput.setString("");
		cursorPosition = 0;
		active = false;
	}

	const sf::Vector2f& TextField::getPosition() const
	{
		// TODO: insert return statement here
	}

	const sf::FloatRect TextField::getGlobalBounds() const
	{
		return sf::FloatRect();
	}

	const std::string& TextField::getCurrentInput()const
	{
		return currentInput.getString();
	}

	const unsigned char TextField::getCharacterSize()const
	{
		return currentInput.getCharacterSize();
	}

	const sf::Color& TextField::getTextColor()const
	{
		return currentInput.getColor();
	}

	TextField& TextField::setCharacterSize(const unsigned char characterSize)
	{
		currentInput.setCharacterSize(characterSize);
		box.setSize(box.getSize().x, sf::Text("|", *currentInput.getFont(), characterSize).getGlobalBounds().height);
		return *this;
	}

	TextField& TextField::setTextColor(const sf::Color& color)
	{
		currentInput.setColor(color);
		return *this;
	}

	TextField& TextField::setPrompt(const std::string& str, const sf::Color& color)
	{
		if (!prompt) prompt.reset(new sf::Text(str, *currentInput.getFont(), currentInput.getCharacterSize()));
		else prompt->setString(str);
		prompt->setColor(color);
		prompt->setPosition(position);
		return *this;
	}

	TextField& TextField::setPromptColor(const sf::Color& color)
	{
		if (!prompt) prompt.reset(new sf::Text("", *currentInput.getFont(), currentInput.getCharacterSize()));
		prompt->setColor(color);
	}

	TextField& TextField::clearPrompt()
	{
		prompt.reset();
		return *this;
	}

	TextField& TextField::setPosition(const float x, const float y)
	{
		position.x = x;
		position.y = y;
		if (prompt) prompt->setPosition(x, y);
		return *this;
	}

	TextField& TextField::setFont(const sf::Font& font)
	{
		currentInput.setFont(font);
		if (prompt) prompt->setFont(font);
		return *this;
	}

	TextField& TextField::setFieldWidth(const unsigned short width)
	{
		box.reset(sf::FloatRect(0, 0, width, box.getSize().y));
		return *this;
	}

	TextField& TextField::setClipboardPermission(const bool permission)
	{
		allowClipboard = permission;
		return *this;
	}

	void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (prompt && currentInput.getString().isEmpty())
		{
			target.draw(*prompt, states);
			return;
		}
		box.setViewport(sf::FloatRect(position.x / target.getSize().x, position.y / target.getSize().y, box.getSize().x / target.getSize().x, box.getSize().y / target.getSize().y));
		const sf::View buffer = target.getView();
		target.setView(box);
		target.draw(currentInput);
		target.setView(buffer);
	}
}