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

#include "../include/GUI/WindowManager.h"
#include "../include/GUI/Hoverable.h"

namespace gui
{
	const Hoverable* WindowManager::m_message = nullptr;

	void WindowManager::input(const sf::Event& event)
	{
		if (m_currentlyDraggedFullscreenWindow && fullscreenWindowInput(*m_currentlyDraggedFullscreenWindow, event)) return;

		if (!m_dialogBoxes.m_elements.empty())
		{
			if (m_dialogBoxes.m_elements.front()->input(event))
			{
				if (m_dialogBoxes.m_elements.front()->isClosed())
				{
					for (auto it(m_dialogBoxes.m_map.begin()), end(m_dialogBoxes.m_map.end()); it != end; ++it)
						if (it->second == &*m_dialogBoxes.m_elements.front())
						{
							m_dialogBoxes.m_map.erase(it);
							break;
						}
					m_dialogBoxes.m_elements.erase(m_dialogBoxes.m_elements.begin());
				}
				else if (!m_dialogBoxes.m_elements.front()->isActive())
					m_dialogBoxes.m_elements.front().swap(m_dialogBoxes.m_elements.back());

				if(event.type == sf::Event::MouseMoved)
					for (const auto& it : m_windows.m_elements)
						it->lostFocus();
					for (auto it = m_dialogBoxes.m_elements.begin() + 1, end = m_dialogBoxes.m_elements.end(); it != end; ++it)
						(*it)->lostFocus();
				return;
			}

			if (event.type == sf::Event::MouseMoved)
				for (auto it = m_dialogBoxes.m_elements.begin() + 1, end = m_dialogBoxes.m_elements.end(); it != end; ++it)
					if ((*it)->input(event))
					{
						for (const auto& it : m_windows.m_elements)
							it->lostFocus();
						return;
					}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				const sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);
				for (auto it = m_dialogBoxes.m_elements.begin() + 1, end = m_dialogBoxes.m_elements.end(); it != end; ++it)
					if ((*it)->contains(position))
					{
						m_dialogBoxes.m_elements.front()->lostFocus();
						m_dialogBoxes.m_elements.front().swap(*it);
						return;
					}
			}
		}
		
		for (auto it = m_windows.m_elements.begin(), end = m_windows.m_elements.end(); it != end; ++it)
			if (fullscreenWindowInput(it, event)) return;
	}

	WindowManager& WindowManager::emplace(const std::string& key, const Window& window, const bool fullscreen)
	{
		(fullscreen ? m_windows : m_dialogBoxes).emplace(key, window);
		(fullscreen ? m_windows : m_dialogBoxes).m_map.at(key)->setParent(this);
		return *this;
	}

	WindowManager& WindowManager::emplace(const std::string& key, Window&& window, const bool fullscreen)
	{
		(fullscreen ? m_windows : m_dialogBoxes).emplace(key, std::move(window));
		(fullscreen ? m_windows : m_dialogBoxes).m_map.at(key)->setParent(this);
		return *this;
	}

	void WindowManager::clear(const bool fullscreen)
	{
		fullscreen ? m_windows.clear() : m_dialogBoxes.clear();
	}
	
	const size_t WindowManager::count(const bool fullscreen) const
	{
		return fullscreen ? m_windows.m_elements.size() : m_dialogBoxes.m_elements.size();
	}

	Window& WindowManager::at(const std::string& key, const bool fullscreen)
	{
		return fullscreen ? *m_windows.m_map.at(key) : *m_dialogBoxes.m_map.at(key);
	}

	const Window& WindowManager::at(const std::string& key, const bool fullscreen) const
	{
		return fullscreen ? *m_windows.m_map.at(key) : *m_dialogBoxes.m_map.at(key);
	}

	void WindowManager::sendToTop(const std::string& key, const bool fullscreen)
	{
		const Window* const target((fullscreen ? m_windows : m_dialogBoxes).m_map.at(key));
		for (auto& it : (fullscreen ? m_windows : m_dialogBoxes).m_elements)
			if (&*it == target)
			{
				it.swap((fullscreen ? m_windows : m_dialogBoxes).m_elements.front());
				break;
			}
	}

	void WindowManager::sendToBottom(const std::string& key, const bool fullscreen)
	{
		const Window* const target((fullscreen ? m_windows : m_dialogBoxes).m_map.at(key));
		for (auto& it : (fullscreen ? m_windows : m_dialogBoxes).m_elements)
			if (&*it == target)
			{
				it.swap((fullscreen ? m_windows : m_dialogBoxes).m_elements.back());
				break;
			}
	}

	void WindowManager::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		for (auto it = m_windows.m_elements.rbegin(), end = m_windows.m_elements.rend(); it != end; ++it)
			target.draw(**it, states);
		for (auto it = m_dialogBoxes.m_elements.rbegin(), end = m_dialogBoxes.m_elements.rend(); it != end; ++it)
			target.draw(**it, states);

		if (m_message)
		{
			m_message->display(target, states);
			m_message = nullptr;
		}
	}

	const bool WindowManager::fullscreenWindowInput(std::vector<std::unique_ptr<gui::Window>>::iterator& window, const sf::Event& event)
	{
		if ((*window)->input(event))
		{
			if ((*window)->isClosed())
			{
				for (auto it1(m_windows.m_map.begin()), end1(m_windows.m_map.end()); it1 != end1; ++it1)
					if (it1->second == &*m_windows.m_elements.front())
					{
						m_windows.m_map.erase(it1);
						break;
					}
				m_windows.m_elements.erase(window);
			}
			else if ((*window)->isBeingMoved())
				m_currentlyDraggedFullscreenWindow.reset(new auto(window));
			else m_currentlyDraggedFullscreenWindow.reset();

			for (const auto& it1 : m_windows.m_elements)
				if (*window != it1) it1->lostFocus();
			for (const auto& it1 : m_dialogBoxes.m_elements)
				it1->lostFocus();
			return true;
		}
		return false;
	}
}