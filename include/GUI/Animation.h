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

#ifndef GUI_ANIMATION_BASE
#define GUI_ANIMATION_BASE

#include <SFML/Graphics/Drawable.hpp>

#include "Internals.h"

namespace gui
{
	class Animation : public virtual sf::Drawable
	{
	public:
		Animation(const Animation& copy) = default;
		Animation(Animation&& copy) = default;
		Animation() = default;
		~Animation() = default;

		Animation& operator=(const Animation& copy) = default;
		Animation& operator=(Animation&& copy) = default;

		virtual Animation& setDuration(const float duration);
		virtual Animation& setFPS(const float fps);

		const float getAnimationDuration()const;
		const float getAnimationFPS()const;

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
		virtual void step()const = 0;

	private:
		void update()const;

		mutable TimePoint m_timeOfLastStep;
		float             m_duration = 0.5f, m_fps = 20.0f;
	};
}

#endif