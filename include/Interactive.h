#ifndef GUI_INTERACTIVE
#define GUI_INTERACTIVE

#include <SFML/Graphics.hpp>
#include <memory>

#include "Internals.h"

namespace gui 
{
	class Interactive : public sf::Drawable
	{
	public:
		virtual ~Interactive() {};

		virtual std::unique_ptr<Interactive> copy()const = 0;
		virtual std::unique_ptr<Interactive> move() = 0;

		virtual const bool input(const sf::Event& event) = 0;

		virtual const sf::FloatRect getGlobalBounds(void) const = 0;
		virtual const sf::Vector2f& getPosition() const = 0;

		virtual Interactive& setPosition(const float x, const float y) = 0;
		virtual Interactive& setPosition(const sf::Vector2f& position) = 0;

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	};
};

#endif