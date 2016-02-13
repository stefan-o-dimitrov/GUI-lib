#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <functional>

#include "Icon.h"
#include "HoverMessage.h"

namespace sf
{
	class Event;
}

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
		ProgressBar(const ProgressBar& copy);

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
		///  Constructs a copy of this gui::ProgressBar from this one and returns a pointer to the new one.
		/// </summary>
		ProgressBar* copy()const override { return new ProgressBar(*this); }

		/// <summary>
		/// Constructs a new gui::ProgressBar, moves this one into it and returns a pointer to the new one, invalidating the original.
		/// </summary>
		ProgressBar* move()override { return new ProgressBar(std::move(*this)); }

		/// <summary>
		/// Allows the progress bar to react to mouse events accordingly.
		/// </summary>
		const bool input(const sf::Event& event)override;

		/// <summary>
		/// Returns the progress of the progress bar, as an integer between 0 and 100.
		/// </summary>
		const float getProgress()const;
		
		/// <summary>
		/// Returns the texture used by the fill Icon.
		/// </summary>
		const sf::Texture& getFillTexture()const;

		/// <summary>
		/// Returns whether or not the fill Icon is performing transparency checks.
		/// </summary>
		const bool getFillTransparencyCheck()const;
		
		/// <summary>
		/// Sets the function, which the gui::ProgressBar should use to update its fill percentage.
		/// </summary>
		ProgressBar& setUpdateFunction(const std::function<const float()>& function);

		/// <summary>
		/// Sets the function, which the gui::ProgressBar should use to update its fill percentage.
		/// </summary>
		ProgressBar& setUpdateFunction(std::function<const float()>&& function);

		/// <summary>
		/// Sets the progress of the progress bar as an integer between 0 and 100.
		/// </summary>
		ProgressBar& setProgress(float percent)const;

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
		/// Sets the position of the progress bar, as the background's most top-left pixel, in screen coordinates.
		/// </summary>
		ProgressBar& setPosition(const sf::Vector2f& pos)override;
		
	private:

		/// <summary>
		/// Draws the progress bar on a given render target with given render states.
		/// </summary>
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		/// <summary>
		/// Pointer to progress update function.
		/// </summary>
		std::unique_ptr<std::function<const float()>> updateFunction;

		/// <summary>
		/// Time of last progress update, in seconds from program start.
		/// </summary>
		mutable float timeOfLastUpdate = 0.0f;

		/// <summary>
		/// Progress of the progress bar, as an integer between 0 and 100.
		/// </summary>
		mutable float progress = 0.0f;

		/// <summary>
		/// Icon for the fill of the progress bar.
		/// </summary>
		mutable Icon fill;

		/// <summary>
		/// How many times the progress percentage should be updated per second.
		/// </summary>
		static const float PROGRESS_UPS;
	};
};