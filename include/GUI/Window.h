#ifndef ELEMENT_CONTAINER
#define ELEMENT_CONTAINER

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>
#include <memory>

#include "Interactive.h"

namespace gui
{
	class Window final : public sf::Drawable
	{
	public:
		Window(const Window& copy) = delete;
		Window(Window&& temp) = default;
		Window() = default;
		~Window() = default;

		const bool input(const sf::Event& event);

		Window& add(Interactive&& element);
		Window& add(const Interactive& element);

		void clear();
		const bool erase(const unsigned short index);
		const size_t size()const;

		Interactive& at(const unsigned short index);
		const Interactive& at(const unsigned short index)const;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

		std::vector<std::unique_ptr<Interactive>> elements;
	};
}

#endif