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

#ifndef GUI_ICON
#define GUI_ICON

#include <memory>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Internals.h"
#include "Interactive.h"
#include "HoverMessage.h"
#include "Hoverable.h"
#include "TransparencyMap.h"

namespace gui 
{
	class Icon : public Hoverable
	{
	public:
		Icon(const sf::Texture& texture, const bool transparecyCheck = false);
		Icon(const Icon& copy);
		Icon(Icon&& temp);
		Icon() = default;
		~Icon() = default;
		
		virtual std::unique_ptr<Interactive> copy()const override;
		virtual std::unique_ptr<Interactive> move()override;

		using Hoverable::lostFocus;
		using Hoverable::input;
		virtual const bool contains(const float x, const float y)const override;
		using Interactive::contains;

		virtual const sf::Vector2f& getPosition()const override;

		virtual const sf::FloatRect getGlobalBounds()const;
		const sf::Texture* getTexture()const;
		const bool getTransparencyCheck()const;
		const sf::IntRect& getTextureRect()const;
		
		virtual Icon& setPosition(const float x, const float y)override;
		virtual Icon& setPosition(const sf::Vector2f& position)override;
		virtual Icon& clearMessage()override;
		virtual Icon& setMessage(const HoverMessage& message)override;
		virtual Icon& setMessage(HoverMessage&& tempMessage)override;

		virtual Icon& setTexture(const sf::Texture& texture, const bool transparencyCheck = false);
		virtual Icon& setTransparencyCheck(const bool transparencyCheck);
		virtual Icon& setTextureRect(const sf::IntRect& textureRect);
		virtual Icon& setColor(const sf::Color& color);

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

		mutable sf::Sprite m_icon;
				
	private:
		std::unique_ptr<TransparencyMap> m_transparency = nullptr;
	};
};

#endif