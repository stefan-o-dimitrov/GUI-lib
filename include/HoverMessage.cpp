#include "HoverMessage.h"
#include "Internals.h"

namespace gui
{
	const unsigned char HoverMessage::TEXT_BOX_X_SPACING = 10, HoverMessage::TEXT_BOX_Y_SPACING = 10;

	HoverMessage::HoverMessage(const ColoredText& str, const sf::Font& font, const unsigned char characterSize)
		: TextPane(str, font, characterSize)
	{
		textBox.setFillColor(sf::Color(0, 0, 0, 255));
		update();
	}

	HoverMessage::HoverMessage(ColoredText&& str, const sf::Font& font, const unsigned char characterSize)
		: TextPane(std::move(str), font, characterSize)
	{
		textBox.setFillColor(sf::Color(0, 0, 0, 255));
		update();
	}

	HoverMessage::HoverMessage(const HoverMessage& copy)
		: TextPane(copy), textBox(copy.textBox)	{}

	HoverMessage::HoverMessage(HoverMessage&& temp)
		: TextPane(std::move(temp)), textBox(temp.textBox) {}

	HoverMessage& HoverMessage::operator=(const HoverMessage& copy)
	{
		TextPane::operator=(copy);
		textBox = copy.textBox;
		return *this;
	}

	HoverMessage& HoverMessage::operator=(HoverMessage&& temp)
	{
		TextPane::operator=(std::move(temp));
		textBox = temp.textBox;
		return *this;
	}

	const sf::FloatRect HoverMessage::getGlobalBounds()const
	{
		return textBox.getGlobalBounds();
	}

	const sf::Color& HoverMessage::getBackgroundFill()const
	{
		return textBox.getFillColor();
	}

	const sf::Color& HoverMessage::getBorderFill()const
	{
		return textBox.getOutlineColor();
	}

	const char HoverMessage::getBorderThickness()const
	{
		return textBox.getOutlineThickness();
	}

	void HoverMessage::updateBox()const
	{
		const sf::FloatRect rect = TextPane::getGlobalBounds();
		textBox.setSize(sf::Vector2f(rect.width + TEXT_BOX_X_SPACING * 2, rect.height + TEXT_BOX_Y_SPACING * 2));
	}

	void HoverMessage::update() const
	{
		TextPane::update();
		updateBox();
	}

	const sf::Vector2f& HoverMessage::getPosition()const
	{
		return position;
	}

	HoverMessage& HoverMessage::setPosition(const float x, const float y)
	{
		position.x = x;
		position.y = y;
		textBox.setPosition(position);
		return *this;
	}

	HoverMessage& HoverMessage::setPosition(const sf::Vector2f& pos)
	{
		return setPosition(pos.x, pos.y);
	}
	
	HoverMessage& HoverMessage::setBackgroundFill(const sf::Color& color)
	{
		textBox.setFillColor(color);
		return *this;
	}

	HoverMessage& HoverMessage::setBorderFill(const sf::Color& color)
	{
		textBox.setOutlineColor(color);
		return *this;
	}

	HoverMessage& HoverMessage::setBorderThickness(const char thickness)
	{
		textBox.setOutlineThickness(thickness);
		return *this;
	}
	
	void HoverMessage::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		sf::Vector2f pos = sf::Vector2f(position.x + textBox.getGlobalBounds().width > target.getSize().x ? 
			(target.getSize().x - textBox.getGlobalBounds().width) - position.x : 0,
			position.y + textBox.getGlobalBounds().height > target.getSize().y ? 
			(target.getSize().y - textBox.getGlobalBounds().height) - position.y : 0);

		states.transform.translate(pos);
		target.draw(textBox, states);
		states.transform.translate(TEXT_BOX_X_SPACING, TEXT_BOX_Y_SPACING);
		TextPane::draw(target, states);
	}
}