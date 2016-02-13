#include "Interactive.h"
#include <SFML/Window/Event.hpp>

#include "Window.h"

gui::Window& gui::Window::add(Interactive&& element)
{
	elements.emplace_back(element.move());
	return *this;
}

const bool gui::Window::input(const sf::Event& event)
{
	for (auto it = elements.rbegin(), end = elements.rend(); it != end; ++it)
		if (*it && (*it)->input(event))
		{
			elements.begin()->swap(*it);
			return true;
		}
	return false;
}

void gui::Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = elements.begin(), end = elements.end(); it != end; ++it)
		target.draw(**it, states);
}
