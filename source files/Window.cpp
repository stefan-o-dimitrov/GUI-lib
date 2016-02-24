#include "../include/GUI/Window.h"

#include <SFML/Window/Event.hpp>

namespace gui
{
	Window& Window::add(Interactive&& element)
	{
		elements.emplace_back(element.move());
		return *this;
	}

	Window& Window::add(const Interactive& element)
	{
		elements.emplace_back(element.copy());
		return *this;
	}

	void Window::clear()
	{
		elements.clear();
	}

	const bool Window::erase(const unsigned short index)
	{
		if (index >= elements.size()) return false;
		elements.erase(elements.begin() + index);
		return true;
	}

	const size_t Window::size() const
	{
		return elements.size();
	}

	Interactive& Window::at(const unsigned short index)
	{
		return *elements.at(index);
	}

	const Interactive& Window::at(const unsigned short index) const
	{
		return *elements.at(index);
	}

	const bool Window::input(const sf::Event& event)
	{
		for (auto it = elements.begin(), end = elements.end(); it != end; ++it)
			if (*it)
			{
				if ((*it)->input(event)) 
				{
					elements.begin()->swap(*it);
					return event.type == sf::Event::MouseButtonPressed;
				}
			}
			else it = elements.erase(it);
		return false;
	}

	void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto it = elements.rbegin(), end = elements.rend(); it != end; ++it)
			if(*it) target.draw(**it, states);
	}
}
