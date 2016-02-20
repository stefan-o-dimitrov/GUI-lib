#include "HoverMessage.h"
#include "Internals.h"

namespace gui
{
	const unsigned char HoverMessage::TEXT_UPS = 10, HoverMessage::TEXT_BOX_X_SPACING = 10, HoverMessage::TEXT_BOX_Y_SPACING = 10;

	HoverMessage::HoverMessage(const ColoredText& str, const sf::Font& font, const unsigned char characterSize)
		: font(&font), characterSize(characterSize), string(str)
	{
		textBox.setFillColor(sf::Color(0, 0, 0, 255));
		string.getText(text, font, characterSize);
		updateBox();
	}

	HoverMessage::HoverMessage(ColoredText&& str, const sf::Font& font, const unsigned char characterSize)
		: font(&font), characterSize(characterSize), string(std::move(str))
	{
		textBox.setFillColor(sf::Color(0, 0, 0, 255));
		string.getText(text, font, characterSize);
		updateBox();
	}

	HoverMessage::HoverMessage(const HoverMessage& copy)
		: position(copy.position), textBox(copy.textBox), font(copy.font), string(copy.string)
	{
		for (auto it = copy.text.begin(), end = copy.text.end(); it != end; ++it)
			text.emplace_back(new sf::Text(*(*it)));
	}

	HoverMessage::HoverMessage(HoverMessage&& temp)
		: position(temp.position), textBox(temp.textBox), text(std::move(temp.text)), font(temp.font), string(std::move(temp.string)) {}

	HoverMessage& HoverMessage::operator=(const HoverMessage& copy)
	{
		position = copy.position;
		textBox = copy.textBox;
		font = copy.font;
		string = copy.string;
		for (auto it = copy.text.begin(), end = copy.text.end(); it != end; ++it)
			text.emplace_back(new sf::Text(*(*it)));
		return *this;
	}

	HoverMessage& HoverMessage::operator=(HoverMessage&& temp)
	{
		position = temp.position;
		textBox = temp.textBox;
		text = std::move(temp.text);
		font = temp.font;
		string = std::move(temp.string);
		return *this;
	}

	const sf::FloatRect HoverMessage::getGlobalBounds()const
	{
		return textBox.getGlobalBounds();
	}

	const sf::Font& HoverMessage::getFont()const
	{
		return *font;
	}

	const unsigned char HoverMessage::getCharacterSize()const
	{
		return characterSize;
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
		sf::Vector2f rectSize(0, 0);
		for (auto it = text.begin(), end = text.end(); it != end; ++it)
		{
			if (rectSize.x < (*it)->getPosition().x + (*it)->getGlobalBounds().width)
				rectSize.x = (*it)->getPosition().x + (*it)->getGlobalBounds().width;
			if (rectSize.y < (*it)->getPosition().y + (*it)->getGlobalBounds().height)
				rectSize.y = (*it)->getPosition().y + (*it)->getGlobalBounds().height;
		}
		textBox.setSize(sf::Vector2f(rectSize.x + TEXT_BOX_X_SPACING * 2, rectSize.y + TEXT_BOX_Y_SPACING * 2));
	}

	const sf::Vector2f& HoverMessage::getPosition()const
	{
		return position;
	}

	HoverMessage& HoverMessage::setText(const ColoredText& newText)
	{
		string = newText;
		string.getText(text, *font, characterSize);
		updateBox();
		return *this;
	}

	HoverMessage& HoverMessage::setText(ColoredText&& val)
	{
		string = std::move(val);
		string.getText(text, *font, characterSize);
		updateBox();
		return *this;
	}

	HoverMessage& HoverMessage::setFont(const sf::Font& newFont)
	{
		font = &newFont;
		for (auto it = text.begin(), end = text.end(); it != end; ++it)
			(*it)->setFont(newFont);
		return *this;
	}

	HoverMessage& HoverMessage::setCharacterSize(const unsigned char newCharacterSize)
	{
		characterSize = newCharacterSize;
		ColoredText::arrangeText(text);
		updateBox();
		return *this;
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

	HoverMessage& HoverMessage::setPosition(const float x, const float y)
	{
		position.x = x;
		position.y = y;
		return *this;
	}

	HoverMessage& HoverMessage::setPosition(const sf::Vector2f& pos)
	{
		return setPosition(pos.x, pos.y);
	}

	void HoverMessage::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		if (string.isVolatile() && Duration(Internals::timeSinceStart() - timeOfLastUpdate).count() > 1.0f / TEXT_UPS)
		{
			string.getText(text, *font, characterSize);
			updateBox();
			timeOfLastUpdate = Internals::timeSinceStart();
		}

		sf::Vector2f pos = position;
		if (position.x + textBox.getGlobalBounds().width > target.getSize().x) pos.x = target.getSize().x - textBox.getGlobalBounds().width;
		if (position.y + textBox.getGlobalBounds().height > target.getSize().y) pos.y = target.getSize().y - textBox.getGlobalBounds().height;

		states.transform.translate(pos);
		target.draw(textBox, states);
		states.transform.translate(TEXT_BOX_X_SPACING, TEXT_BOX_Y_SPACING);
		for (auto it = text.begin(), end = text.end(); it != end; ++it)
			target.draw(**it, states);
	}
}