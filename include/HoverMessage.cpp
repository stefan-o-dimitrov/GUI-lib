#include "HoverMessage.h"

const unsigned char gui::HoverMessage::TEXT_BOX_X_SPACING = 10, gui::HoverMessage::TEXT_BOX_Y_SPACING = 10;

gui::HoverMessage::HoverMessage(const ColoredString& string, const sf::Font& font, const unsigned char characterSize)
	: font(&font), characterSize(characterSize)
{
	setText(string);
	const sf::FloatRect rectSize = getGlobalBounds();
	textBox.setSize(sf::Vector2f(rectSize.width + TEXT_BOX_X_SPACING * 2, rectSize.height + TEXT_BOX_Y_SPACING * 2));
}

gui::HoverMessage::HoverMessage(const HoverMessage& _lVal)
	: position(_lVal.position), textBox(_lVal.textBox)
{
	for (auto it = _lVal.text.begin(), end = _lVal.text.end(); it != end; ++it)
		text.push_back(std::unique_ptr<sf::Text>(new sf::Text(*(*it))));
}

gui::HoverMessage::HoverMessage(HoverMessage&& _rVal)
	: position(_rVal.position), textBox(_rVal.textBox), text(std::move(_rVal.text)) {}

gui::HoverMessage & gui::HoverMessage::operator=(const HoverMessage& _lVal)
{
	position = _lVal.position;
	textBox = _lVal.textBox;
	for (auto it = _lVal.text.begin(), end = _lVal.text.end(); it != end; ++it)
		text.push_back(std::unique_ptr<sf::Text>(new sf::Text(*(*it))));
	return *this;
}

gui::HoverMessage& gui::HoverMessage::operator=(HoverMessage&& _rVal)
{
	position = _rVal.position;
	textBox = _rVal.textBox;
	text = std::move(_rVal.text);
	return *this;
}

const sf::FloatRect gui::HoverMessage::getGlobalBounds()const
{
	return textBox.getGlobalBounds();
}

const sf::Font& gui::HoverMessage::getFont()const
{
	return *font;
}

const unsigned char gui::HoverMessage::getCharacterSize()const
{
	return characterSize;
}

const sf::Color& gui::HoverMessage::getBackgroundFill()const
{
	return textBox.getFillColor();
}

const sf::Color& gui::HoverMessage::getBorderFill()const
{
	return textBox.getOutlineColor();
}

const char gui::HoverMessage::getBorderThickness()const
{
	return textBox.getOutlineThickness();
}

const sf::Vector2f& gui::HoverMessage::getPosition()const
{
	return textBox.getPosition();
}

gui::HoverMessage& gui::HoverMessage::setText(const ColoredString& string)
{
	text = std::move(ColoredString::interpret(string, *font, characterSize));
	return *this;
}

gui::HoverMessage& gui::HoverMessage::setFont(const sf::Font& _font)
{
	font = &_font;
	for (auto it = text.begin(), end = text.end(); it != end; ++it)
		(*it)->setFont(_font);
	return *this;
}

gui::HoverMessage& gui::HoverMessage::setCharacterSize(const unsigned char _characterSize)
{
	characterSize = _characterSize;
	for (auto it = text.begin(), end = text.end(); it != end; ++it)
		(*it)->setCharacterSize(_characterSize);
	return *this;
}

gui::HoverMessage& gui::HoverMessage::setBackgroundFill(const sf::Color& color)
{
	textBox.setFillColor(color);
	return *this;
}

gui::HoverMessage& gui::HoverMessage::setBorderFill(const sf::Color& color)
{
	textBox.setOutlineColor(color);
	return *this;
}

gui::HoverMessage& gui::HoverMessage::setBorderThickness(const char thickness)
{
	textBox.setOutlineThickness(thickness);
	return *this;
}

gui::HoverMessage& gui::HoverMessage::setPosition(const float x, const float y)
{
	position.x = x;
	position.y = y;
	return *this;
}

gui::HoverMessage& gui::HoverMessage::setPosition(const sf::Vector2f& pos)
{
	return setPosition(pos.x, pos.y);
}

void gui::HoverMessage::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	sf::Vector2f pos = position;
	if (textBox.getPosition().x + textBox.getGlobalBounds().width > target.getSize().x) pos.x = target.getSize().x - textBox.getGlobalBounds().width;
	if (textBox.getPosition().y + textBox.getGlobalBounds().height > target.getSize().y) pos.y = target.getSize().y - textBox.getGlobalBounds().height;

	states.transform.translate(pos);

	target.draw(textBox, states);

	states.transform.translate(TEXT_BOX_X_SPACING, TEXT_BOX_Y_SPACING);

	states.transform.translate(position);
	for (auto it = text.begin(), end = text.end(); it != end; ++it)
		target.draw(**it, states);
}
