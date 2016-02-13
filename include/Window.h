#ifndef ELEMENT_CONTAINER
#define ELEMENT_CONTAINER

#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <memory>

namespace sf
{
	class Event;
}

namespace gui
{
	class Interactive;
	class Window : public sf::Drawable
	{
	public:
		Window(const Window& copy) = delete;
		Window(Window&& temp) = default;
		Window() = default;
		~Window() = default;

		void clear();
		Window& add(Interactive&& element);
		Window& add(const Interactive& element);
		const bool input(const sf::Event& event);
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		std::vector<std::unique_ptr<Interactive>> elements;
	};
}

#endif