#include "Internals.h"
#include "AudioSystem.h"

sf::Clock gui::Internals::clock;

void gui::Internals::loadResources(const std::string& musiceFilesTxtDir, const std::string& UISoundFilesTxtDir, const std::string& gameSoundFilesTxtDir)
{
	AudioSystem::loadResources(musiceFilesTxtDir, UISoundFilesTxtDir, gameSoundFilesTxtDir);
}

const sf::Clock& gui::Internals::getClock()
{
	return clock;
}