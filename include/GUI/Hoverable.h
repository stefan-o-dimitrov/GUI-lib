#ifndef GUI_HOVERABLE_BASE
#define GUI_HOVERABLE_BASE

#include <memory>

#include "HoverMessage.h"
#include "Interactive.h"

namespace gui
{
	class Hoverable : public virtual Interactive
	{
	public:
		virtual const bool input(const sf::Event& event)override;
		virtual const bool contains(const sf::Vector2f& point)const = 0;

		virtual const std::shared_ptr<const HoverMessage> getMessage()const;

		virtual Hoverable& clearMessage();
		virtual Hoverable& setMessage(const HoverMessage& message);
		virtual Hoverable& setMessage(HoverMessage&& messageTemp);
		Hoverable& setDelay(const float delaySeconds);

	protected:		
		Hoverable(const HoverMessage& message, const float delay = 0.75f);
		Hoverable(const Hoverable& copy);
		Hoverable(Hoverable&& temp) = default;
		Hoverable() = default;

		Hoverable& operator=(const Hoverable& copy);
		Hoverable& operator=(Hoverable&& temp) = default;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

		void mouseEntered(const sf::Vector2f& position);
		void mouseLeft();
		const bool hasMessageDelayPassed()const;
		mutable std::shared_ptr<HoverMessage> message = nullptr;
		
	private:		
		mutable bool messageDelayPassed = false;
		float delay = 0.75f;
		bool hasBeenPressed = false;
		std::unique_ptr<TimePoint> timeMouseEntered = nullptr;
	};
}

#endif