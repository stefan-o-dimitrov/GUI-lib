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

#include "Window.h"

namespace gui
{
	class WindowManager final : public sf::Drawable
	{
	public:
		void input(const sf::Event& event);

		WindowManager& pushBack(const Window& window, const bool fullscreen = false);
		WindowManager& pushBack(Window&& window, const bool fullscreen = false);
		WindowManager& pushFront(const Window& window, const bool fullscreen = false);
		WindowManager& pushFront(Window&& window, const bool fullscreen = false);

		void clear(const bool fullscreen = false);
		const bool erase(const unsigned short index, const bool fullscreen = false);
		const size_t size(const bool fullscreen = false)const;

		Window& at(const unsigned short index, const bool fullscreen = false);
		const Window& at(const unsigned short index, const bool fullscreen = false)const;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states)const;

		std::vector<std::unique_ptr<Window>> windows, dialogBoxes;
	};
}

#endif