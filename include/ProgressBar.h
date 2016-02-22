#ifndef GUI_PROGRESS_BAR
#define GUI_PROGRESS_BAR

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <functional>

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

		std::unique_ptr<Interactive> copy()const override;
		std::unique_ptr<Interactive> move()override;

		const bool input(const sf::Event& event)override;

		const float getProgress()const;
		const sf::Texture& getFillTexture()const;
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

		std::unique_ptr<std::function<const float()>> updateFunction;
		mutable TimePoint timeOfLastUpdate;
		mutable float progress = 0.0f;
		mutable Icon fill;
	};
}

#endif