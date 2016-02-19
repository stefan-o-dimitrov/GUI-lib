#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

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

	class Button : public Icon
	{
		friend class CheckBox;
	public:
		typedef std::vector<std::pair<std::function<const bool()>, ColoredString>>(predicateArray);

		Button(const Icon& visual, const std::function<void()>& onClick);
		Button(Icon&& visual, std::function<void()>&& onClick);
		Button(const Button& copy);
		Button(Button&& temp) = default;
		Button() = default;
		~Button() = default;

		virtual Button* copy()const override { return new Button(*this); }
		virtual Button* move()override { return new Button(std::move(*this)); };

		virtual const bool input(const sf::Event& event)override;

		virtual const State getState()const;
		const std::shared_ptr<const HoverMessage> getPredicateMessage()const;
		const std::shared_ptr<const HoverMessage> getMessage()const override;
		const std::shared_ptr<const TextArea> getName()const;
		const std::shared_ptr<const unsigned short> getClickSound()const;

		Button& setName(const TextArea& name);
		Button& setName(TextArea&& nameTemp);
		Button& setClickSound(const unsigned short soundKey);
		Button& setMessage(const HoverMessage& message)override;
		Button& setMessage(HoverMessage&& messageTemp)override;
		virtual Button& setPosition(const float x, const float y)override;
		Button& setPredicates(const predicateArray& predicates, const sf::Font& font, const unsigned char characterSize = 13);

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

	private:

		void checkPredicates()const;
		const bool arePredicatesFulfilled()const;

		mutable std::shared_ptr<HoverMessage>  messageBuffer = nullptr;
		mutable std::unique_ptr<ColoredString> stringBuffer = nullptr;
		std::unique_ptr<predicateArray>        predicates = nullptr;
		std::shared_ptr<unsigned short>        sound = nullptr;
		std::shared_ptr<TextArea>              name = nullptr;

		mutable sf::Shader                     stateShader;
		std::function<void()>                  onClickAction;
		float                                  timeOfLastPredicateCheck = 0.0f;
		State                                  state = Idle;
		mutable bool                           predicatesFulfilled = true;
		
		static const std::string STATE_SHADER;
		static const unsigned char PREDICATE_CHECKS_PER_SECOND;
	};
};
