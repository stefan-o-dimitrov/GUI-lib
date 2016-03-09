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

#include <SFML/Window/Event.hpp>

#include "../include/GUI/Window.h"

namespace gui
{
	Window::Window(const Window& copy)
		: background(copy.background), movable(copy.movable)
	{
		copy.transparency ? transparency.reset(new TransparencyMap(*copy.transparency)) : 0;
		copy.mouseDragOffset ? mouseDragOffset.reset(new sf::Vector2f(*copy.mouseDragOffset)) : 0;

		elements.reserve(copy.elements.size());
		for (auto it = copy.elements.begin(), end = copy.elements.end(); it != end; ++it)
			elements.emplace_back(std::move((*it)->copy()));
	}

	Window& Window::add(Interactive&& element)
	{
		elements.emplace_back(element.move());
		elements.back()->setPosition(elements.back()->getPosition().x + background.getPosition().x,
			elements.back()->getPosition().y + background.getPosition().y);
		return *this;
	}

	Window& Window::add(const Interactive& element)
	{
		elements.emplace_back(element.copy());
		elements.back()->setPosition(elements.back()->getPosition().x + background.getPosition().x,
			elements.back()->getPosition().y + background.getPosition().y);
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

	Window& Window::setPosition(const sf::Vector2f& position)
	{
		return setPosition(position.x, position.y);
	}

	Window& Window::setPosition(const float x, const float y)
	{
		const sf::Vector2f moveAmount(x - background.getPosition().x, y - background.getPosition().y);
		background.setPosition(x, y);

		for (auto it = elements.begin(), end = elements.end(); it != end; ++it)
			(*it)->setPosition((*it)->getPosition().x + moveAmount.x,
				(*it)->getPosition().y + moveAmount.y);
		return *this;
	}

	Window& Window::setBackgroundTexture(const sf::Texture& texture, const bool transparencyCheck)
	{
		background.setTexture(texture);
		return *this;
	}

	Window& Window::setTransparencyCheck(const bool transparencyCheck)
	{
		transparencyCheck ? transparency.reset(new TransparencyMap(*background.getTexture())) : transparency.reset();
		return *this;
	}

	Window& Window::setBackgroundTextureRect(const sf::IntRect& textureRect)
	{
		background.setTextureRect(textureRect);
		return *this;
	}

	Window& Window::setMovable(const bool isMovable)
	{
		movable = isMovable;
		return *this;
	}
	
	const bool Window::contains(const sf::Vector2f& point) const
	{
		if (background.getGlobalBounds().contains(point))
		{
			if (!transparency) return true;
			if (!(*transparency)[sf::Vector2i(point.x - background.getPosition().x, point.y - background.getPosition().y)]) return true;
		}
		return false;
	}

	const bool Window::input(const sf::Event& event)
	{
		for (auto it = elements.begin(), end = elements.end(); it != end; ++it)
			if ((*it)->input(event)) 
			{
				elements.begin()->swap(*it);
				return true;
			}

		if (movable)
			switch (event.type)
			{
			case sf::Event::MouseMoved:
				if (mouseDragOffset)
					setPosition(sf::Vector2f(event.mouseMove.x - mouseDragOffset->x, event.mouseMove.y - mouseDragOffset->y));
				return true;
			case sf::Event::MouseButtonPressed:
				if (contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
					mouseDragOffset.reset(new sf::Vector2f(event.mouseButton.x - getPosition().x,
						event.mouseButton.y - getPosition().y));
				return true;
			case sf::Event::MouseButtonReleased:
				mouseDragOffset.reset();
				return true;
			}

		return false;
	}

	const sf::Vector2f& Window::getPosition() const
	{
		return background.getPosition();
	}

	const bool Window::isMovable() const
	{
		return movable;
	}

	void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(background, states);
		for (auto it = elements.rbegin(), end = elements.rend(); it != end; ++it)
			if(*it) target.draw(**it, states);
	}
}
