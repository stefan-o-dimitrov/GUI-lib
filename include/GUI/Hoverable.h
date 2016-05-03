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

#ifndef GUI_HOVERABLE_BASE
#define GUI_HOVERABLE_BASE

#include <memory>

#include "HoverMessage.h"
#include "Interactive.h"

namespace gui
{
	class Hoverable : public Interactive
	{
	public:
		Hoverable(const HoverMessage& message, const float delay = 0.75f);
		Hoverable(const Hoverable& copy);
		Hoverable(Hoverable&& temp) = default;
		Hoverable() = default;

		Hoverable& operator=(const Hoverable& copy);
		Hoverable& operator=(Hoverable&& temp) = default;

		virtual void lostFocus()override;
		virtual const bool input(const sf::Event& event)override;
		using Interactive::contains;

		virtual const std::shared_ptr<HoverMessage> getMessage()const;

		virtual Hoverable& clearMessage();
		virtual Hoverable& setMessage(const HoverMessage& message);
		virtual Hoverable& setMessage(HoverMessage&& messageTemp);
		Hoverable& setDelay(const float delaySeconds);

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

		void mouseEntered(const sf::Vector2f& position);
		void mouseLeft();
		const bool hasMessageDelayPassed()const;

		mutable std::shared_ptr<HoverMessage> m_message = nullptr;
		
	private:
		float                          m_delay = 0.75f;
		bool                           m_hasBeenPressed = false;
		std::unique_ptr<TimePoint>     m_timeMouseEntered = nullptr;
	};
}

#endif