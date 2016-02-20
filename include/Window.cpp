#include "Interactive.h"
#include "Window.h"

#include <SFML/Window/Event.hpp>

namespace gui
{
	void Window::clear()
	{
		elements.clear();
	}

	Window& Window::add(Interactive&& element)
	{
		elements.emplace_back(element.move());
		return *this;
	}

	Window& Window::add(const Interactive & element)
	{
		elements.emplace_back(element.copy());
		return *this;
	}

	const bool Window::input(const sf::Event& event)
	{
		for (auto it = elements.rbegin(), end = elements.rend(); it != end; ++it)
			if (*it && (*it)->input(event))
			{
				elements.rbegin()->swap(*it);
				return true;
			}
		return false;
	}

	void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto it = elements.begin(), end = elements.end(); it != end; ++it)
			target.draw(**it, states);
	}
}
