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

#ifndef GUI_CHECK_BOX
#define GUI_CHECK_BOX

#include "Button.h"

namespace gui
{
	class CheckBox final : public Button
	{
	public:
		CheckBox(const Button& falseState, const Button& trueState, const sf::Vector2f& position = sf::Vector2f(0, 0), const bool state = false);
		CheckBox(Button&& falseState, Button&& trueState, const sf::Vector2f& position = sf::Vector2f(0, 0), const bool state = false);
		CheckBox(const CheckBox& copy) = default;
		CheckBox(CheckBox&& temp) = default;
		CheckBox() = default;
		~CheckBox() = default;

		std::unique_ptr<Interactive> copy()const override;
		std::unique_ptr<Interactive> move()override;

		void lostFocus()override;
		const bool input(const sf::Event& event)override;
		const bool contains(const float x, const float y)const override;
		using Interactive::contains;

		void toggle();

		const State getState()const override;
		const bool isChecked()const;

		const sf::FloatRect getTrueStateGlobalBounds()const;
		const sf::Texture& getTrueStateTexture()const;
		const bool getTrueStateTransparencyCheck()const;
		const sf::IntRect& getTrueStateTextureRect()const;
		const std::shared_ptr<const HoverMessage> getTrueStateMessage()const;
		const std::shared_ptr<const HoverMessage> getTrueStatePredicateMessage()const;
		
		CheckBox& setPosition(const float x, const float y)override;
		CheckBox& setPosition(const sf::Vector2f& position)override;
		CheckBox& setIsChecked(const bool isChecked);

		CheckBox& setTrueStateTexture(const sf::Texture& texture, const bool transparencyCheck = false);
		CheckBox& setTrueStateTransparencyCheck(const bool transparencyCheck);
		CheckBox& setTrueStateTextureRect(const sf::IntRect& textureRect);
		CheckBox& setTrueStateMessage(const HoverMessage& message);
		CheckBox& setTrueStateMessage(HoverMessage&& tempMessage);
		CheckBox& setTrueStatePredicateMessage(const HoverMessage& message);
		CheckBox& setTrueStatePredicateMessage(HoverMessage&& tempMessage);
		CheckBox& setTrueStatePredicates(const PredicateArray& predicates);
		CheckBox& setTrueStatePredicates(PredicateArray&& predicates);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
		
		using Button::getName;
		using Button::setName;

		Button m_trueState;
		bool   m_checked = false;
	};
};

#endif;