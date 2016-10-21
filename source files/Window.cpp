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

#include "../include/GUI/WindowManager.h"
#include "../include/GUI/Window.h"
#include "../include//GUI/Hoverable.h"

#include <iostream>

namespace gui
{
	const Hoverable* Window::m_message = nullptr;

	template class ordered_map<gui::Interactive>;
	template class ordered_map<gui::Window>;

	template<typename StorageType>
	ordered_map<StorageType>::ordered_map(const ordered_map& copy)
	{
		for (const auto& it : copy.m_map)
			emplace(it.first, *it.second);
	}

	template<typename StorageType>
	ordered_map<StorageType>& ordered_map<StorageType>::emplace(const std::string& key, const StorageType& element)
	{
		m_elements.emplace_back(std::move(element.copy()));
		m_map.emplace(key, &*m_elements.back());

		return *this;
	}

	template<typename StorageType>
	ordered_map<StorageType>& ordered_map<StorageType>::emplace(const std::string& key, StorageType&& element)
	{
		m_elements.emplace_back(std::move(element.move()));
		m_map.emplace(key, &*m_elements.back());

		return *this;
	}

	template<typename StorageType>
	void ordered_map<StorageType>::clear()
	{
		m_map.clear();
		m_elements.clear();
	}
	
	Window::Window(const Window& copy)
		: m_background(copy.m_background), m_movable(copy.m_movable), m_active(copy.m_active), m_closed(copy.m_closed), m_elements(copy.m_elements)
	{
		copy.m_mouseDragOffset ? m_mouseDragOffset.reset(new sf::Vector2f(*copy.m_mouseDragOffset)) : 0;
	}

	Window& Window::add(const std::string& key, Interactive&& element)
	{
		m_elements.emplace(key, std::move(element));
		m_elements.m_map.at(key)->setParent(this);

		m_elements.m_elements.back()->setPosition(m_elements.m_elements.back()->getPosition().x + m_background.getPosition().x,
			m_elements.m_elements.back()->getPosition().y + m_background.getPosition().y);

		return *this;
	}

	Window& Window::add(const std::string& key, const Interactive& element)
	{
		m_elements.emplace(key, element);
		m_elements.m_map.at(key)->setParent(this);

		m_elements.m_elements.back()->setPosition(m_elements.m_elements.back()->getPosition().x + m_background.getPosition().x,
			m_elements.m_elements.back()->getPosition().y + m_background.getPosition().y);

		return *this;
	}

	void Window::clear()
	{
		m_elements.clear();
	}

	Window& Window::setActive(const bool active)
	{
		m_active = active;
		return *this;
	}

	const bool Window::isActive()
	{
		return m_active;
	}

	const bool Window::isClosed()
	{
		return m_closed;
	}

	const size_t Window::count() const
	{
		return m_elements.m_elements.size();
	}

	const bool Window::exists(const std::string& key)
	{
		return m_elements.m_map.count(key);
	}

	Interactive& Window::at(const std::string& key)
	{
		return *m_elements.m_map.at(key);
	}

	const Interactive& Window::at(const std::string& key) const
	{
		return *m_elements.m_map.at(key);
	}

	void Window::erase(const std::string& key)
	{
		const auto target(m_elements.m_map.at(key));
		for (auto it(m_elements.m_elements.begin()), end(m_elements.m_elements.end()); it != end; ++it)
			if (&*(*it) == target)
			{
				m_elements.m_elements.erase(it);
				m_elements.m_map.erase(key);
				return;
			}
	}

	void Window::close()
	{
		m_closed = true;
	}

	Window& Window::setPosition(const sf::Vector2f& position)
	{
		return setPosition(position.x, position.y);
	}

	Window& Window::setPosition(const float x, const float y)
	{
		const sf::Vector2f moveAmount(x - m_background.getPosition().x, y - m_background.getPosition().y);
		m_background.setPosition(x, y);

		for (const auto& it : m_elements.m_elements)
			it->setPosition(it->getPosition().x + moveAmount.x,
				it->getPosition().y + moveAmount.y);
		return *this;
	}

	Window& Window::setBackgroundTexture(const sf::Texture& texture, const bool transparencyCheck)
	{
		m_background.setTexture(texture);
		return *this;
	}

	Window& Window::setTransparencyCheck(const bool transparencyCheck)
	{
		m_background.setTransparencyCheck(transparencyCheck);
		return *this;
	}

	Window& Window::setBackgroundTextureRect(const sf::IntRect& textureRect)
	{
		m_background.setTextureRect(textureRect);
		return *this;
	}

	Window& Window::setBackgroundColor(const sf::Color& color)
	{
		m_background.setColor(color);
		return *this;
	}

	Window& Window::setMovable(const bool isMovable)
	{
		m_movable = isMovable;
		return *this;
	}
	
	std::unique_ptr<Window> Window::copy() const
	{
		return std::unique_ptr<Window>(new Window(*this));
	}

	std::unique_ptr<Window> Window::move()
	{
		return std::unique_ptr<Window>(new Window(std::move(*this)));
	}

	const bool Window::contains(const sf::Vector2f& point) const
	{
		return m_background.contains(point);
	}

	const bool Window::input(const sf::Event& event)
	{
		bool returnValue(false);

		if (!m_active) return returnValue;

		for (auto& it : m_elements.m_elements)
			if (it->input(event)) 
			{
				m_elements.m_elements.begin()->swap(it);
				returnValue = true;
			}

		if ((!returnValue || event.type == sf::Event::MouseButtonReleased) && m_movable)
			switch (event.type)
			{
			case sf::Event::MouseMoved:
			{
				if (m_mouseDragOffset)
				{
					setPosition(sf::Vector2f(event.mouseMove.x - m_mouseDragOffset->x, event.mouseMove.y - m_mouseDragOffset->y));
					return true;
				}
				return contains(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
			}
			case sf::Event::MouseButtonPressed:
			{
				if (contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
				{
					m_mouseDragOffset.reset(new sf::Vector2f(event.mouseButton.x - getPosition().x,
						event.mouseButton.y - getPosition().y));
					return true;
				}
				return returnValue;
			}
			case sf::Event::MouseButtonReleased:
				m_mouseDragOffset.reset();
				return contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
			}
					
		if (!returnValue && m_background.getTexture() && ((event.type == sf::Event::MouseMoved && m_background.contains(event.mouseMove.x, event.mouseMove.y)) ||
			((event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) &&
				m_background.contains(event.mouseButton.x, event.mouseButton.y)))) return true;

		return returnValue;
	}
	
	void Window::lostFocus()
	{
		for (const auto& it : m_elements.m_elements)
			it->lostFocus();
	}

	const sf::Vector2f& Window::getPosition() const
	{
		return m_background.getPosition();
	}

	const bool Window::isMovable() const
	{
		return m_movable;
	}

	const bool Window::isBeingMoved() const
	{
		return bool(m_mouseDragOffset);
	}

	void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!m_active) return;

		target.draw(m_background, states);
		for (auto it = m_elements.m_elements.rbegin(), end = m_elements.m_elements.rend(); it != end; ++it)
			target.draw(**it, states);
		
		if (m_message)
		{
			if (m_parent && m_message && m_message->getMessage()) WindowManager::m_message = m_message;
			else m_message->display(target, states);
			
			if (!m_message->getMessage() || (m_message->getMessage() && m_message->getMessage()->getFadeAmount() == 0.0f && m_message->getMessage()->getFadeDirection() == 0))
				m_message = nullptr;
		}
	}

	void Window::setParent(const WindowManager* const parent)
	{
		m_parent = (WindowManager*)parent;
	}

	const Icon& Window::background() const
	{
		return m_background;
	}

	std::unordered_map<std::string, Interactive*const>::const_iterator Window::begin() const
	{
		return m_elements.m_map.begin();
	}

	std::unordered_map<std::string, Interactive*const>::const_iterator Window::end() const
	{
		return m_elements.m_map.end();
	}

	std::unordered_map<std::string, Interactive*const>::iterator Window::begin()
	{
		return m_elements.m_map.begin();
	}

	std::unordered_map<std::string, Interactive*const>::iterator Window::end()
	{
		return m_elements.m_map.end();
	}
}
