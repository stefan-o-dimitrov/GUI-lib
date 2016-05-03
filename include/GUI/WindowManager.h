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

#ifndef GUI_WINDOW_MANAGER
#define GUI_WINDOW_MANAGER

#include <unordered_map>

#include "Window.h"

namespace gui
{
	class WindowManager final : public sf::Drawable
	{
	public:
		void input(const sf::Event& event);

		WindowManager& emplace(const std::string& key, const Window& window, const bool fullscreen = false);
		WindowManager& emplace(const std::string& key, Window&& window, const bool fullscreen = false);

		void clear(const bool fullscreen = false);
		const size_t size(const bool fullscreen = false)const;

		Window& at(const std::string& key, const bool fullscreen);
		const Window& at(const std::string& key, const bool fullscreen)const;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const;

		const bool fullscreenWindowInput(decltype(ordered_map<Window>::m_elements.begin())& window, const sf::Event& event);

		ordered_map<Window> m_windows, m_dialogBoxes;
		std::unique_ptr<decltype(m_windows.m_elements.begin())> m_currentlyDraggedFullscreenWindow = nullptr;
	};
}

#endif