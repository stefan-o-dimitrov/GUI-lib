#ifndef GUI_BUTTON
#define GUI_BUTTON

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
		static sf::Shader          shader;
		static const std::string   STATE_SHADER;
	};
};

#endif