////////////////////////////////////////////////////////////
//
// GUI-lib - Graphical User Interface Library
// Copyright (C) 2014-2016 ShtGames (shtgamessts@gmail.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef GUI_TEXT_PANE
#define GUI_TEXT_PANE

#include <memory>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Internals.h"
#include "Interactive.h"
#include "ColoredText.h"

namespace gui 
{
	class TextPane : public Interactive
	{
	public:
		TextPane(const ColoredText& text, const sf::Font& font, const unsigned char characterSize = 13);
		TextPane(ColoredText&& text, const sf::Font& font, const unsigned char characterSize = 13);
		TextPane(const TextPane& copy);
		TextPane(TextPane&& temp) = default;
		TextPane() = default;
		~TextPane() = default;

		TextPane& operator=(const TextPane& copy);
		TextPane& operator=(TextPane&& temp) = default;

		virtual std::unique_ptr<Interactive> copy()const override;
		virtual std::unique_ptr<Interactive> move()override;

		const bool input(const sf::Event& event)override;
		const bool contains(const sf::Vector2f& point)const;
		
		const sf::FloatRect getGlobalBounds()const;
		virtual const sf::Vector2f& getPosition()const;
		virtual const sf::Font& getFont()const;
		virtual const unsigned char getCharacterSize()const;

		virtual TextPane& setPosition(const float x, const float y) override;
		virtual TextPane& setPosition(const sf::Vector2f& position) override;

		virtual TextPane& setText(const ColoredText& text);
		virtual TextPane& setText(ColoredText&& text);
		virtual TextPane& setFont(const sf::Font& font);
		virtual TextPane& setCharacterSize(const unsigned char characterSize);

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
		virtual void update()const;

		sf::Vector2f position = sf::Vector2f(0, 0);

	private:
		mutable unique_ptr_vector<sf::Text> text;
		const sf::Font* font;
		unsigned char characterSize = 13;
		ColoredText string;
		mutable TimePoint timeOfLastUpdate;
	};
}

#endif
