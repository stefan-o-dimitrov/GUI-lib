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
#include <unordered_map>

#include <SFML/Graphics/Shader.hpp>

#include "Internals.h"
#include "Interactive.h"
#include "TextArea.h"
#include "Icon.h"

namespace gui 
{

	class Button : public Icon
	{
	public:
		enum State
		{
			Hot,
			Idle,
			PressedDown
		};

		enum Event
		{
			Pressed,
			Released,
			PredicatesFulfilled,
			PredicatesUnfulfilled
		};

		typedef std::vector<std::function<const bool()>>(PredicateArray);

		Button(const Icon& visual);
		Button(Icon&& visual);
		Button(const Button& copy);
		Button(Button&& temp) = default;
		Button() = default;
		~Button() = default;

		virtual std::unique_ptr<Interactive> copy()const override;
		virtual std::unique_ptr<Interactive> move()override;

		virtual void lostFocus()override;
		virtual const bool input(const sf::Event& event)override;
		using Icon::contains;
		
		const std::shared_ptr<const HoverMessage> getMessage()const override;

		virtual const State getState()const;
		const std::shared_ptr<const HoverMessage> getPredicateMessage()const;
		const std::shared_ptr<const TextArea> getName()const;

		virtual Button& setPosition(const float x, const float y)override;
		virtual Button& setPosition(const sf::Vector2f& position)override;
		Button& clearMessage()override;
		Button& setMessage(const HoverMessage& message)override;
		Button& setMessage(HoverMessage&& tempMessage)override;

		Button& setName(const TextArea& name);
		Button& setName(TextArea&& nameTemp);
		Button& clearPredicateMessage();
		Button& setPredicateMessage(const HoverMessage& message);
		Button& setPredicateMessage(HoverMessage&& tempMessage);
		Button& clearPredicates();
		Button& setPredicates(const PredicateArray& predicates);
		Button& setPredicates(PredicateArray&& predicates);
		Button& bindAction(const Event event, const std::function<void()>& action);
		Button& bindAction(const Event event, std::function<void()>&& action);

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

	private:
		typedef std::unordered_map<Event, std::function<void()>>(EventMap);

		void checkPredicates()const;

		mutable std::shared_ptr<HoverMessage>   messageBuffer = nullptr;
		mutable std::unique_ptr<PredicateArray> predicates = nullptr;
		std::shared_ptr<TextArea>               name = nullptr;
		EventMap                                onEvent;

		TimePoint    timeOfLastPredicateCheck;
		State        state = Idle;
		mutable bool predicatesFulfilled = true;

		static const bool loadShader();

		static const bool shaderLoadSuccessful;
		static sf::Shader grayscaleShader;

		friend class CheckBox;
	};
};

#endif