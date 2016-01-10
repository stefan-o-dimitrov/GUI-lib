#include "TextField.h"

template <typename processTextInputFunction>
gui::TextField<processTextInputFunction>::TextField(const std::function<processTextInputFunction>& processTextInputFunction, const unsigned short fieldWidth, const sf::Font& font, const unsigned char characterSize)
	: processingFunction(processTextInputFunction), currentInput("", font, characterSize)
{
	box.reset(sf::FloatRect(0, 0, fieldWidth, sf::Text("|", font, characterSize).getGlobalBounds().height));
}

template <typename processTextInputFunction>
gui::TextField<processTextInputFunction>::TextField(const TextField& source)
	: box(source.box), currentInput(source.currentInput), processingFunction(source.processingFunction), allowClipboard(source.allowClipboard)
{
	if (source.prompt) prompt.reset(new TextArea(source.prompt));
}

template <typename processTextInputFunction>
gui::TextField<processTextInputFunction>& gui::TextField<processTextInputFunction>::operator=(const TextField<processTextInputFunction>& _lVal)
{
	box = _lVal.box;
	currentInput = _lVal.currentInput;
	processingFunction = _lVal.processingFunction;
	allowClipboard = _lVal.allowClipboard;
	if (_lVal.prompt) prompt.reset(new sf::Text(*_lVal.prompt));
}

template <typename processTextInputFunction>
const bool gui::TextField<processTextInputFunction>::input(const sf::Event& event)
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

template <typename processTextInputFunction>
void gui::TextField<processTextInputFunction>::processCurrentInput()
{
	processingFunction(currentInput.getString());
	currentInput.setString("");
	cursorPosition = 0;
	active = false;
}

template <typename processTextInputFunction>
const std::string& gui::TextField<processTextInputFunction>::getCurrentInput()const
{
	return currentInput.getString();
}

template <typename processTextInputFunction>
const unsigned char gui::TextField<processTextInputFunction>::getCharacterSize()const
{
	return currentInput.getCharacterSize();
}

template <typename processTextInputFunction>
const sf::Color& gui::TextField<processTextInputFunction>::getTextColor()const
{
	return currentInput.getColor();
}

template <typename processTextInputFunction>
gui::TextField<processTextInputFunction>& gui::TextField<processTextInputFunction>::setCharacterSize(const unsigned char characterSize)
{
	currentInput.setCharacterSize(characterSize);
	box.setSize(box.getSize().x, sf::Text("|", *currentInput.getFont(), characterSize).getGlobalBounds().height);
	return *this;
}

template <typename processTextInputFunction>
gui::TextField<processTextInputFunction>& gui::TextField<processTextInputFunction>::setTextColor(const sf::Color& color)
{
	currentInput.setColor(color);
	return *this;
}

template <typename processTextInputFunction>
gui::TextField<processTextInputFunction>& gui::TextField<processTextInputFunction>::setPrompt(const ColoredString& coloredString)
{
	prompt.reset(new sf::Text(coloredString.vec.second, *currentInput.getFont(), currentInput.getCharacterSize()));
	prompt->setColor(coloredString.vec.first);
	prompt->setPosition(position);
	return *this;
}

template <typename processTextInputFunction>
gui::TextField<processTextInputFunction>& gui::TextField<processTextInputFunction>::clearPrompt()
{
	prompt.reset();
	return *this;
}

template <typename processTextInputFunction>
gui::TextField<processTextInputFunction>& gui::TextField<processTextInputFunction>::setPosition(const float x, const float y)
{
	position.x = x;
	position.y = y;
	if (prompt) prompt->setPosition(x, y);
	return *this;
}

template <typename processTextInputFunction>
gui::TextField<processTextInputFunction>& gui::TextField<processTextInputFunction>::setFont(const sf::Font& font)
{
	currentInput.setFont(font);
	if (prompt) prompt->setFont(font);
	return *this;
}

template <typename processTextInputFunction>
gui::TextField<processTextInputFunction>& gui::TextField<processTextInputFunction>::setFieldWidth(const unsigned short width)
{
	box.reset(sf::FloatRect(0, 0, width, box.getSize().y));
	return *this;
}

template <typename processTextInputFunction>
gui::TextField<processTextInputFunction>& gui::TextField<processTextInputFunction>::setClipboardPermission(const bool permission)
{
	allowClipboard = permission;
	return *this;
}

template <typename processTextInputFunction>
void gui::TextField<processTextInputFunction>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (prompt && currentInput.getString().isEmpty())
	{
		target.draw(*prompt, states);
		return;
	}
	box.setViewport(sf::FloatRect(position.x / target.getSize().X, position.y / target.getSize().y, box.getSize().x / target.getSize().x, box.getSize().y / target.getSize().y));
	const sf::View buffer = target.getView();
	target.setView(box);
	target.draw(currentInput);
	target.setView(buffer);
}