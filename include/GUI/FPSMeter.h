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

#ifndef GUI_FPS_METER
#define GUI_FPS_METER

#include <SFML/Graphics/Text.hpp>

#include "Interactive.h"
#include "Internals.h"

namespace gui
{
	class FPSMeter final : public gui::Interactive
	{
	public:
		FPSMeter(const sf::Font& font, const unsigned char characterSize = 13);
		FPSMeter(const FPSMeter& copy) = default;
		FPSMeter(FPSMeter&& temp) = default;
		FPSMeter() = default;
		~FPSMeter() = default;

		FPSMeter& operator=(const FPSMeter& copy) = default;
		FPSMeter& operator=(FPSMeter&& temp) = default;

		std::unique_ptr<Interactive> copy()const override;
		std::unique_ptr<Interactive> move()override;

		const sf::FloatRect getGlobalBounds() const override;
		const sf::Vector2f& getPosition() const override;

		FPSMeter& setPosition(const float x, const float y)override;
		FPSMeter& setPosition(const sf::Vector2f& position)override;

		const sf::Font& getFont()const;
		const unsigned char getCharacterSize()const;
		const sf::Color& getColor()const;

		FPSMeter& setFont(const sf::Font& font);
		FPSMeter& setCharacterSize(const unsigned char characterSize);
		FPSMeter& setColor(const sf::Color& color);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		mutable unsigned short frames, previousFrames;
		mutable sf::Text text;
		mutable TimePoint timeOfLastUpdate;
	};
}

#endif