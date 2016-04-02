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
		if (!dialogBoxes.empty())
		{
			if (dialogBoxOrder.front()->input(event))
			{
				if(event.type == sf::Event::MouseMoved)
					for (auto it = dialogBoxOrder.begin() + 1, end = dialogBoxOrder.end(); it != end; ++it)
						(*it)->lostFocus();
				return;
			}

			if (event.type == sf::Event::MouseMoved)
				for (auto it = dialogBoxOrder.begin() + 1, end = dialogBoxOrder.end(); it != end; ++it)
					if((*it)->input(event))
						return;

			if (event.type == sf::Event::MouseButtonPressed)
			{
				const sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);
				for (auto it = dialogBoxOrder.begin() + 1, end = dialogBoxOrder.end(); it != end; ++it)
					if ((*it)->contains(position))
					{
						dialogBoxOrder.front()->lostFocus();
						dialogBoxOrder.front().swap(*it);
						return;
					}
			}
		}

		for (auto it = windowOrder.begin(), end = windowOrder.end(); it != end; ++it)
			if ((*it)->input(event)) break;
	}

	WindowManager& WindowManager::pushBack(const Window& window, const bool fullscreen)
	{
		fullscreen ? windows.emplace_back(new Window(window)) : dialogBoxes.emplace_back(new Window(window));

		auto& target = fullscreen ? windowOrder : dialogBoxOrder;

		std::vector<std::shared_ptr<Window>> buffer;
		buffer.reserve(target.size() + 1);
		buffer.push_back(fullscreen ? windows.back() : dialogBoxes.back());

		for (auto it = target.begin(), end = target.end(); it != end; ++it)
			buffer.emplace_back(std::move(*it));

		target = std::move(buffer);

		return *this;
	}

	WindowManager& WindowManager::pushBack(Window&& window, const bool fullscreen)
	{
		fullscreen ? windows.emplace_back(new Window(std::move(window))) :
			dialogBoxes.emplace_back(new Window(std::move(window)));

		auto& target = fullscreen ? windowOrder : dialogBoxOrder;

		std::vector<std::shared_ptr<Window>> buffer;
		buffer.reserve(target.size() + 1);
		buffer.push_back(fullscreen ? windows.back() : dialogBoxes.back());

		for (auto it = target.begin(), end = target.end(); it != end; ++it)
			buffer.emplace_back(std::move(*it));

		target = std::move(buffer);

		return *this;
	}

	WindowManager& WindowManager::pushFront(const Window& window, const bool fullscreen)
	{
		fullscreen ? windows.emplace_back(new Window(window)) : dialogBoxes.emplace_back(new Window(window));
		fullscreen ? windowOrder.push_back(windows.back()) : dialogBoxOrder.push_back(dialogBoxes.back());
		return *this;
	}

	WindowManager& WindowManager::pushFront(Window&& window, const bool fullscreen)
	{
		fullscreen ? windows.emplace_back(new Window(std::move(window))) : dialogBoxes.emplace_back(new Window(std::move(window)));
		fullscreen ? windowOrder.push_back(windows.back()) : dialogBoxOrder.push_back(dialogBoxes.back());
		return *this;
	}

	void WindowManager::clear(const bool fullscreen)
	{
		fullscreen ? windows.clear() : dialogBoxes.clear();
		fullscreen ? windowOrder.clear() : dialogBoxOrder.clear();
	}

	const bool WindowManager::erase(const unsigned short index, const bool fullscreen)
	{
		auto& target = fullscreen ? windows : dialogBoxes;
		if (index >= target.size()) return false;

		auto element(target.begin() + index);
		for (auto it = (fullscreen ? windowOrder : dialogBoxOrder).begin(), end = (fullscreen ? windowOrder : dialogBoxOrder).end(); it != end; ++it)
			if (*it = *element)
			{
				(fullscreen ? windowOrder : dialogBoxOrder).erase(it);
				break;
			}
		target.erase(element);
		return true;
	}

	const size_t WindowManager::size(const bool fullscreen) const
	{
		return fullscreen ? windows.size() : dialogBoxes.size();
	}

	Window& WindowManager::at(const unsigned short index, const bool fullscreen)
	{
		return fullscreen ? *windows.at(index) : *dialogBoxes.at(index);
	}

	const Window& WindowManager::at(const unsigned short index, const bool fullscreen) const
	{
		return fullscreen ? *windows.at(index) : *dialogBoxes.at(index);
	}

	void WindowManager::draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
		for (auto it = windowOrder.rbegin(), end = windowOrder.rend(); it != end; ++it)
			target.draw(**it, states);
		for (auto it = dialogBoxOrder.rbegin(), end = dialogBoxOrder.rend(); it != end; ++it)
			target.draw(**it, states);
	}
}