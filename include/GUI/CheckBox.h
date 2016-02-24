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

		const bool input(const sf::Event& event)override;
		const bool contains(const sf::Vector2f& point)const;

		const State getState()const override;
		const bool isChecked()const;

		const sf::FloatRect getTrueStateGlobalBounds()const;
		const sf::Texture& getTrueStateTexture()const;
		const bool getTrueStateTransparencyCheck()const;
		const sf::IntRect& getTrueStateTextureRect()const;
		const std::shared_ptr<const HoverMessage> getTrueStateMessage()const;
		const std::shared_ptr<const HoverMessage> getTrueStatePredicateMessage()const;
		
		void toggle();

		CheckBox& setPosition(const float x, const float y)override;
		CheckBox& setPosition(const sf::Vector2f& position)override;

		CheckBox& setIsChecked(const bool isChecked);
		CheckBox& setTrueStateTexture(const sf::Texture& texture, const bool transparencyCheck = false);
		CheckBox& setTrueStateTransparencyCheck(const bool transparencyCheck);
		CheckBox& setTrueStateTextureRect(const sf::IntRect& textureRect);
		CheckBox& setTrueStateClickSound(const unsigned short soundKey);
		CheckBox& setTrueStateMessage(const HoverMessage& message);
		CheckBox& setTrueStateMessage(HoverMessage&& messageTemp);
		CheckBox& setTrueStatePredicates(const PredicateArray& predicates);
		CheckBox& setTrueStatePredicates(PredicateArray&& predicates);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
		
		using Button::getName;
		using Button::setName;

		Button trueState;
		bool checked = false;
	};
};

#endif;