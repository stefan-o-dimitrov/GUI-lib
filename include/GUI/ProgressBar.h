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

#ifndef GUI_PROGRESS_BAR
#define GUI_PROGRESS_BAR

#include <memory>
#include <functional>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

#include "Icon.h"

namespace gui 
{
	class ProgressBar final : public Icon
	{
	public:
		ProgressBar(const Icon& background, const Icon& fill, const unsigned char progress = 0);
		ProgressBar(Icon&& background, Icon&& fill, const unsigned char progress = 0);
		ProgressBar(const ProgressBar& copy);
		ProgressBar(ProgressBar&& temp) = default;
		ProgressBar() = default;
		~ProgressBar() = default;

		ProgressBar& operator=(const ProgressBar& copy);
		ProgressBar& operator=(ProgressBar&& temp) = default;

		std::unique_ptr<Interactive> copy()const override;
		std::unique_ptr<Interactive> move()override;

		void lostFocus()override;
		const bool input(const sf::Event& event)override;
		const bool contains(const float x, const float y)const override;
		using Interactive::contains;

		const float getProgress()const;
		const sf::Texture* getFillTexture()const;
		const bool getFillTransparencyCheck()const;
		const std::shared_ptr<const HoverMessage> getFillMessage()const;

		ProgressBar& setPosition(const float x, const float y)override;
		ProgressBar& setPosition(const sf::Vector2f& position)override;

		ProgressBar& setUpdateFunction(const std::function<const float()>& function);
		ProgressBar& setUpdateFunction(std::function<const float()>&& function);
		ProgressBar& setProgress(float percent)const;
		ProgressBar& setFillTexture(const sf::Texture& texture, const bool transparencyCheck = false);
		ProgressBar& setFillTransparencyCheck(const bool transparencyCheck);		

		ProgressBar& clearFillMessage();
		ProgressBar& setFillMessage(const HoverMessage& message);
		ProgressBar& setFillMessage(HoverMessage&& messageTemp);
		ProgressBar& setFillDelay(const float delaySeconds);
		
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		std::unique_ptr<std::function<const float()>> m_updateFunction = nullptr;
		mutable TimePoint                             m_timeOfLastUpdate;
		mutable float                                 m_progress = 0.0f;
		mutable Icon                                  m_fill;
	};
}

#endif