#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "HoverMessage.h"
#include "Icon.h"
#include "Interactive.h"

namespace gui 
{
	/// <summary>
	/// Specialized class for handling progress bars in a GUI environment, with optional 
	/// message box on mouseover and transparency checks for both background and fill.
	/// </summary>
	class ProgressBar final : public Icon
	{
	public:
		/// <summary>
		/// Constructs a progress bar from given Icons for the background and fill, respectively, by making copies of both, as well as the fill progress, as an integer between 0 and 100.
		/// </summary>
		ProgressBar(const Icon& background, const Icon& fill, const unsigned char progress = 0);

		/// <summary>
		/// Constructs a progress bar from given Icons for the background and fill, respectively, by moving both, as well as the fill progress, as an integer between 0 and 100.
		/// </summary>
		ProgressBar(Icon&& background, Icon&& fill, const unsigned char progress = 0);

		/// <summary>
		/// Constructs a progress bar by copying the given one.
		/// </summary>
		ProgressBar(const ProgressBar& copy) = default;

		/// <summary>
		/// Constructs a progress bar by moving the given one.
		/// </summary>
		ProgressBar(ProgressBar&& temp) = default;

		/// <summary>
		/// Constructs and empty Progress Bar.
		/// </summary>
		ProgressBar() = default;

		/// <summary>
		/// Destructor.
		/// </summary>
		~ProgressBar() = default;

		/// <summary>
		/// Constructs a new ProgressBar, moves this one into it and returns a pointer to the new one, invalidating the original.
		/// </summary>
		ProgressBar* move()override { return new ProgressBar(std::move(*this)); }

		/// <summary>
		/// Allows the progress bar to react to mouse events accordingly.
		/// </summary>
		const bool input(const sf::Event& event)override;

		/// <summary>
		/// Returns the progress of the progress bar, as an integer between 0 and 100.
		/// </summary>
		const unsigned char getProgress()const;
		
		/// <summary>
		/// Returns the texture used by the fill Icon.
		/// </summary>
		const sf::Texture& getFillTexture()const;

		/// <summary>
		/// Returns whether or not the fill Icon is performing transparency checks.
		/// </summary>
		const bool getFillTransparencyCheck()const;

		/// <summary>
		/// Returns the Hover Message that is drawn when the mouse is hovering over the fill of the progress bar.
		/// </summary>
		const std::shared_ptr<const HoverMessage> getFillMessage()const;

		/// <summary>
		/// Sets the progress of the progress bar as an integer between 0 and 100.
		/// </summary>
		ProgressBar& setProgress(const unsigned char percent);

		/// <summary>
		/// Sets the fill texture of the progress bar, with optional specification of whether or not transparency checks should be performed on it.
		/// </summary>
		ProgressBar& setFillTexture(const sf::Texture& texture, const bool transparencyCheck = false);

		/// <summary>
		/// Sets whether or not transparency checks should be performed on the fill Icon.
		/// </summary>
		ProgressBar& setFillTransparencyCheck(const bool transparencyCheck);

		/// <summary>
		/// Sets the position of the progress bar, as the background's most top-left pixel, in screen coordinates.
		/// </summary>
		ProgressBar& setPosition(const float x, const float y)override;

		/// <summary>
		/// Sets the hover message to draw when the mouse is hovering over the fill of the progress bar.
		/// </summary>
		ProgressBar& setFillMessage(const HoverMessage& message);

		/// <summary>
		/// Sets the hover message to draw when the mouse is hovering over the fill of the progress bar.
		/// </summary>
		ProgressBar& setFillMessage(HoverMessage&& messageTemp);

	private:

		/// <summary>
		/// Draws the progress bar on a given render target with given render states.
		/// </summary>
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		/// <summary>
		/// Progress of the progress bar, as an integer between 0 and 100.
		/// </summary>
		unsigned char progress = 0;

		/// <summary>
		/// Icon for the fill of the progress bar.
		/// </summary>
		Icon fill;
	};
};