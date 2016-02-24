#ifndef GUI_ICON
#define GUI_ICON

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <memory>

#include "Internals.h"
#include "Interactive.h"
#include "HoverMessage.h"
#include "Hoverable.h"

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

		using Hoverable::input;

		virtual const bool contains(const sf::Vector2f& point)const override;

		virtual const sf::Vector2f& getPosition()const override;

		virtual const sf::FloatRect getGlobalBounds()const;
		const sf::Texture& getTexture()const;
		const bool getTransparencyCheck()const;
		const sf::IntRect& getTextureRect()const;
		
		virtual Icon& setPosition(const float x, const float y)override;
		virtual Icon& setPosition(const sf::Vector2f& position)override;
		virtual Icon& clearMessage()override;
		virtual Icon& setMessage(const HoverMessage& message)override;
		virtual Icon& setMessage(HoverMessage&& tempMessage)override;

		Icon& setTexture(const sf::Texture& texture, const bool transparencyCheck = false);
		Icon& setTransparencyCheck(const bool transparencyCheck);
		Icon& setTextureRect(const sf::IntRect& textureRect);


	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

		mutable sf::Sprite spr;
				
	private:
		class TransparencyMap
		{
		public:
			TransparencyMap(const sf::Texture& texture);
			TransparencyMap(const TransparencyMap& copy);
			TransparencyMap(TransparencyMap&& temp) = default;

			const bool operator[](const sf::Vector2i& coordinates)const;

			void generateTransparencyMap(const sf::Texture& texture);

		private:
			std::unique_ptr<std::unique_ptr<bool[]>[]> transparency = nullptr;
			sf::Vector2i mapSize = sf::Vector2i(0, 0);
		};

		std::unique_ptr<TransparencyMap> transparency = nullptr;
	};
};

#endif