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

#ifndef GUI_BUTTON
#define GUI_BUTTON

#include <functional>
#include <iostream>

#include <SFML/Graphics/Shader.hpp>

#include "Internals.h"
#include "Interactive.h"
#include "TextArea.h"
#include "Icon.h"

namespace gui 
{
	enum State
	{
		Hot,
		Idle,
		PressedDown
	};
	typedef std::vector<std::function<const bool()>>(PredicateArray);

	class Button : public Icon
	{
		friend class CheckBox;
	public:

		Button(const Icon& visual, const std::function<void()>& onClick);
		Button(Icon&& visual, std::function<void()>&& onClick);
		Button(const Button& copy);
		Button(Button&& temp) = default;
		Button() = default;
		~Button() = default;

		virtual std::unique_ptr<Interactive> copy()const override;
		virtual std::unique_ptr<Interactive> move()override;

		virtual const bool input(const sf::Event& event)override;
		
		const std::shared_ptr<const HoverMessage> getMessage()const override;

		virtual const State getState()const;
		const std::shared_ptr<const HoverMessage> getPredicateMessage()const;
		const std::shared_ptr<const TextArea> getName()const;
		const std::shared_ptr<const unsigned short> getClickSound()const;

		virtual Button& setPosition(const float x, const float y)override;
		virtual Button& setPosition(const sf::Vector2f& position)override;
		Button& clearMessage()override;
		Button& setMessage(const HoverMessage& message)override;
		Button& setMessage(HoverMessage&& tempMessage)override;

		Button& setName(const TextArea& name);
		Button& setName(TextArea&& nameTemp);
		Button& setClickSound(const unsigned short soundKey);
		Button& setPredicateMessage(const HoverMessage& message);
		Button& setPredicateMessage(HoverMessage&& tempMessage);
		Button& setPredicates(const PredicateArray& predicates);
		Button& setPredicates(PredicateArray&& predicates);

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

	private:

		const bool arePredicatesFulfilled()const;

		mutable std::shared_ptr<HoverMessage>   messageBuffer = nullptr;
		mutable std::unique_ptr<ColoredText>    stringBuffer = nullptr;
		mutable std::unique_ptr<PredicateArray> predicates = nullptr;
		std::shared_ptr<unsigned short>         sound = nullptr;
		std::shared_ptr<TextArea>               name = nullptr;

		std::function<void()> onClickAction;
		TimePoint             timeOfLastPredicateCheck;
		State                 state = Idle;
		mutable bool          predicatesFulfilled = true;

		static const bool loadShader();

		static const bool          shaderLoadSuccessful;
		static sf::Shader          grayscaleShader;
	};
};

#endif