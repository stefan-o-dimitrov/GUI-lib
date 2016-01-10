#pragma once
#include <SFML/Graphics.hpp>
#include "HoverMessage.h"

namespace gui
{
	class Internals 
	{
	public:
		static void loadResources(const std::string& musiceFilesTxtDir,
			const std::string& UISoundFilesTxtDir,
			const std::string& gameSoundFilesTxtDir);
		static const sf::Clock& getClock();
	private:
		static sf::Clock clock;
	};
};