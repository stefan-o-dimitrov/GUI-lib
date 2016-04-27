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

namespace gui
{
	void WindowManager::input(const sf::Event& event)
	{
		if (!m_dialogBoxOrder.empty())
		{
			if (m_dialogBoxOrder.front()->input(event))
			{
				if (m_dialogBoxOrder.front()->isClosed())
				{
					for (auto it(m_dialogBoxes.begin()), end(m_dialogBoxes.end()); it != end; ++it)
						if (it->second == &*m_dialogBoxOrder.front())
						{
							m_windows.erase(it);
							break;
						}
					m_dialogBoxOrder.erase(m_dialogBoxOrder.begin());
				}
				else if (!m_dialogBoxOrder.front()->isActive())
					m_dialogBoxOrder.front().swap(m_dialogBoxOrder.back());

				if(event.type == sf::Event::MouseMoved)
					for (auto it = m_dialogBoxOrder.begin() + 1, end = m_dialogBoxOrder.end(); it != end; ++it)
						(*it)->lostFocus();
				return;
			}

			if (event.type == sf::Event::MouseMoved)
				for (auto it = m_dialogBoxOrder.begin() + 1, end = m_dialogBoxOrder.end(); it != end; ++it)
					if((*it)->input(event))
						return;

			if (event.type == sf::Event::MouseButtonPressed)
			{
				const sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);
				for (auto it = m_dialogBoxOrder.begin() + 1, end = m_dialogBoxOrder.end(); it != end; ++it)
					if ((*it)->contains(position))
					{
						m_dialogBoxOrder.front()->lostFocus();
						m_dialogBoxOrder.front().swap(*it);
						return;
					}
			}
		}

		for (auto it = m_windowOrder.begin(), end = m_windowOrder.end(); it != end; ++it)
			if ((*it)->input(event))
			{
				if ((*it)->isClosed())
				{
					for (auto it(m_windows.begin()), end(m_windows.end()); it != end; ++it)
						if (it->second == &*m_windowOrder.front())
						{
							m_windows.erase(it);
							break;
						}
					m_windowOrder.erase(it);
				}
				m_windowOrder.front().swap(*it);
				break;
			}
	}

	WindowManager& WindowManager::emplace(const std::string& key, const Window& window, const bool fullscreen)
	{
		(fullscreen ? m_windows : m_dialogBoxes).emplace(key, new Window(window));
		(fullscreen ? m_windowOrder : m_dialogBoxOrder).emplace_back(&*(fullscreen ? m_windows : m_dialogBoxes).at(key));
		return *this;
	}

	WindowManager& WindowManager::emplace(const std::string& key, Window&& window, const bool fullscreen)
	{
		(fullscreen ? m_windows : m_dialogBoxes).emplace(key, new Window(std::move(window)));
		(fullscreen ? m_windowOrder : m_dialogBoxOrder).emplace_back(&*(fullscreen ? m_windows : m_dialogBoxes).at(key));
		return *this;
	}

	void WindowManager::clear(const bool fullscreen)
	{
		fullscreen ? m_windows.clear() : m_dialogBoxes.clear();
	}
	
	const size_t WindowManager::size(const bool fullscreen) const
	{
		return fullscreen ? m_windows.size() : m_dialogBoxes.size();
	}

	Window& WindowManager::at(const std::string& key, const bool fullscreen)
	{
		return fullscreen ? *m_windows.at(key) : *m_dialogBoxes.at(key);
	}

	const Window& WindowManager::at(const std::string& key, const bool fullscreen) const
	{
		return fullscreen ? *m_windows.at(key) : *m_dialogBoxes.at(key);
	}

	void WindowManager::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		for (auto it = m_windowOrder.rbegin(), end = m_windowOrder.rend(); it != end; ++it)
			target.draw(**it, states);
		for (auto it = m_dialogBoxOrder.rbegin(), end = m_dialogBoxOrder.rend(); it != end; ++it)
			target.draw(**it, states);
	}
}