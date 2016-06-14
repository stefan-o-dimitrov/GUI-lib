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

#ifndef GUI_WINDOW
#define GUI_WINDOW

#include <vector>
#include <memory>
#include <unordered_map>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>

#include "Interactive.h"
#include "Icon.h"

namespace gui
{
	template <typename StorageType>
	struct ordered_map final
	{
		ordered_map(const ordered_map& copy);
		ordered_map(ordered_map&& temp) = default;
		ordered_map() = default;
		~ordered_map() = default;

		ordered_map& operator=(ordered_map&& temp) = default;

		ordered_map& emplace(const std::string& key, const StorageType& element);
		ordered_map& emplace(const std::string& key, StorageType&& element);

		void clear();
		
		std::unordered_map<std::string, StorageType* const> m_map;
		std::vector<std::unique_ptr<StorageType>>           m_elements;
	};

	class Window : public sf::Drawable
	{
		friend class Hoverable;
		friend class WindowManager;
	public:
		Window(const Window& copy);
		Window(Window&& temp) = default;
		Window() = default;
		virtual ~Window() = default;

		Window& operator=(Window&& temp) = default;

		virtual std::unique_ptr<Window> copy()const;
		virtual std::unique_ptr<Window> move();

		const bool contains(const sf::Vector2f& point)const;
		virtual const bool input(const sf::Event& event);
		virtual void lostFocus();

		const sf::Vector2f& getPosition()const;
		const bool isMovable()const;
		const bool isBeingMoved()const;

		Window& setPosition(const sf::Vector2f& position);
		Window& setPosition(const float x, const float y);
		Window& setBackgroundTexture(const sf::Texture& texture, const bool transparencyCheck = false);
		Window& setTransparencyCheck(const bool transparencyCheck);
		Window& setBackgroundTextureRect(const sf::IntRect& textureRect);
		Window& setBackgroundColor(const sf::Color& color);
		Window& setMovable(const bool isMovable);

		Window& add(const std::string& key, Interactive&& element);
		Window& add(const std::string& key, const Interactive& element);

		virtual void clear();

		Window& setActive(const bool active);
		const bool isActive();
		void close();
		const bool isClosed();
		const size_t count()const;

		const bool exists(const std::string& key);

		Interactive& at(const std::string& key);
		const Interactive& at(const std::string& key)const;
		void erase(const std::string& key);

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
		virtual void setParent(const WindowManager* const parent);

		const Icon& background()const;
		std::unordered_map<std::string, Interactive* const>::const_iterator begin()const;
		std::unordered_map<std::string, Interactive* const>::const_iterator end()const;
		std::unordered_map<std::string, Interactive* const>::iterator begin();
		std::unordered_map<std::string, Interactive* const>::iterator end();

		static const Hoverable* m_message;

	private:
		Icon                          m_background;
		ordered_map<Interactive>      m_elements;
		bool                          m_movable = false, m_active = true, m_closed = false;
		std::unique_ptr<sf::Vector2f> m_mouseDragOffset = nullptr;
		WindowManager*                m_parent = nullptr;
	};
}

#endif