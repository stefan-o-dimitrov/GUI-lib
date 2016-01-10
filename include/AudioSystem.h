#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>

namespace gui
{
	class AudioSystem
	{
	public:
		typedef unsigned char(volume);

		static void loadResources(const std::string& musicDefinitionsDir, const std::string& interfaceSoundDefinitionsDir, const std::string& gameSoundDefinitionsDir);
		static void playInterfaceSound(const unsigned short);
		static void playGameSound(const unsigned short);
		static void playSong(const unsigned short);
		static void playRandomSong();
		static void setMusicVolume(const volume);
		static void setInterfaceVolume(const volume);
		static void setSoundVolume(const volume);
		static void setMasterVolume(const volume);
		static const volume getMusicVolume();
		static const volume getInterfaceVolume();
		static const volume getGameVolume();
		static const volume getMasterVolume();
		static const sf::Sound& getInterfaceSound(const unsigned short);
		static const sf::Sound& getGameSound(const unsigned short);
	private:
		static void loadMusic(const std::string&);
		static void loadInterfaceSound(const std::string&);
		static void loadGameSound(const std::string&);

		static volume                                                                    musicVolume, interfaceVolume, gameVolume, masterVolume;
		static unsigned short                                                            currentSong;
		static std::unordered_map<unsigned short, sf::Music*>                            music;
		static std::unordered_map<unsigned short, std::pair<sf::Sound, sf::SoundBuffer>> interfaceSound, gameSound;
	};
};