#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Internals.h"
#include "Interactive.h"
#include "HoverMessage.h"
#include "Hoverable.h"

namespace gui 
{
	/// <summary>
	/// Class for handling image elements in a GUI environment,
	/// with optional message box on mouseover and transparency check.
	/// </summary>
	class Icon : public Hoverable
	{
	public:
		/// <summary>
		/// Constructs an Icon from given texture.
		/// </summary>
		Icon(const sf::Texture& texture, const bool transparecyCheck = false);

		/// <summary>
		/// Constructs an Icon by making a deep copy of the given one.
		/// </summary>
		Icon(const Icon& copy);

		/// <summary>
		/// Constructs and Icon by moving the temp Icon given.
		/// </summary>
		Icon(Icon&& temp);

		/// <summary>
		/// Constructs and empty Icon.
		/// </summary>
		Icon() = default;

		/// <summary>
		/// Destructor.
		/// </summary>
		~Icon() = default;

		/// <summary>
		/// Constructs a copy of this gui::Icon from this one and returns a pointer to the new one.
		/// </summary>
		virtual Icon* copy()const override { return new Icon(*this); };

		/// <summary>
		/// Constructs a new gui::Icon, moves this one into it and returns a pointer to the new one, invalidating this instance.
		/// </summary>
		virtual Icon* move()override { return new Icon(std::move(*this)); };

		/// <summary>
		/// Allows the Icon to react to mouse events accordingly.
		/// </summary>
		using Hoverable::input;

		/// <summary>
		/// Checks whether or not the given point in screen coordinates is contained within the Icon, taking into account transparency if set to true.
		/// </summary>
		virtual const bool contains(const sf::Vector2f& point)const override;

		/// <summary>
		/// Returns the smallest, axis-aligned bounding rectangle of the Icon, in screen coordinates, without taking transparency into account.
		/// </summary>
		virtual const sf::FloatRect getGlobalBounds()const;

		/// <summary>
		/// Returns the position of the Icon, as its most top-left pixel, in screen coordinates.
		/// </summary>
		virtual const sf::Vector2f& getPosition()const override;

		/// <summary>
		/// Returns a constant reference to the texture used by the Icon.
		/// </summary>
		const sf::Texture& getTexture()const;

		/// <summary>
		/// Returns whether or not the Icon should perform transparency checks.
		/// </summary>
		const bool getTransparencyCheck()const;

		/// <summary>
		/// Returns a rectangle representing the portion of the given texture 
		/// to draw relative to its most top-left pixel.
		/// </summary>
		const sf::IntRect& getTextureRect()const;
		
		/// <summary>
		/// Sets the position of the Icon, as its most top-left pixel, in screen coordinates.
		/// </summary>
		virtual Icon& setPosition(const float x, const float y)override;

		/// <summary>
		/// Sets the texture of the Icon, with the option to specify whether or not it should perform transparency checks on it.
		/// </summary>
		Icon& setTexture(const sf::Texture& texture, const bool transparencyCheck = false);

		/// <summary>
		/// Sets whether or not the Icon should perform transparency checks.
		/// </summary>
		Icon& setTransparencyCheck(const bool transparencyCheck);
		
		/// <summary>
		/// Sets the portion of the given texture to draw,
		/// relative to its most top-left pixel.
		/// </summary>
		Icon& setTextureRect(const sf::IntRect& textureRect);

	protected:

		/// <summary>
		/// Draws the Icon on a given render target with the given render states.
		/// </summary>
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

		/// <summary>
		/// Sprite containing the Icon image.
		/// </summary>
		mutable sf::Sprite spr;
				
	private:

		/// <summary>
		/// Specialized class for handling transparency in a given texture.
		/// </summary>
		class TransparencyMap
		{
		public:

			/// <summary>
			/// Constructs a transparency map from a given source texture.
			/// </summary>
			TransparencyMap(const sf::Texture& texture);

			/// <summary>
			/// Constructs a transparency map by making a deep copy of the given one.
			/// </summary>
			TransparencyMap(const TransparencyMap& copy);

			/// <summary>
			/// Constructs a transparency map by moving the given one.
			/// </summary>
			TransparencyMap(TransparencyMap&& temp) = default;
			
			/// <summary>
			/// Returns whether or not the pixel at the given texture coordinates is transparent.
			/// </summary>
			const bool operator[](const sf::Vector2i& coordinates)const;

			/// <summary>
			/// Generates the transparency map from a given source texture.
			/// </summary>
			void generateTransparencyMap(const sf::Texture& texture);

		private:

			/// <summary>
			/// Transparency of each pixel of the source texture in a 2D dynamic array.
			/// </summary>
			std::unique_ptr<std::unique_ptr<bool[]>[]> transparency = nullptr;

			/// <summary>
			/// Size of the source texture, in pixels.
			/// </summary>
			sf::Vector2i mapSize = sf::Vector2i(0, 0);
		};

		/// <summary>
		/// Pointer to the transparency map for the Icon to use.
		/// </summary>
		std::unique_ptr<TransparencyMap> transparency = nullptr;
	};
};