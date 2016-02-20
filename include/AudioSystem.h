#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>

namespace gui
{
	typedef unsigned char(Volume);

	class AudioSystem
	{
	public:

		const bool loadMusicFile(const unsigned short key, const std::string& path);
		const bool loadSoundFile(const unsigned short key, const std::string& path);

		static void playSound(const unsigned short key);
		static void playSong(const unsigned short key);
		static void playRandomSong();

		static void setMusicVolume(const Volume volume);
		static void setSoundVolume(const Volume volume);
		static void setMasterVolume(const Volume volume);
		static void mute(const bool muted);

		static const Volume getMusicVolume();
		static const Volume getSoundVolume();
		static const Volume getMasterVolume();
		static const bool isSongPlaying();
		static const bool isMuted();

	private:

		static Volume                                                                    musicVolume, soundVolume, masterVolume;
		static bool                                                                      muted;
		static unsigned short                                                            currentSong;
		static std::unordered_map<unsigned short, std::unique_ptr<sf::Music>>            music;
		static std::unordered_map<unsigned short, std::pair<sf::Sound, sf::SoundBuffer>> sound;
	};
};