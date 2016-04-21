////////////////////////////////////////////////////////////
//
// GUI-lib - Graphical User Interface Library
// Copyright (C) 2014-2016 ShtGames (shtgamessts@gmail.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef GUI_AUDIO_SYSTEM
#define GUI_AUDIO_SYSTEM

#include <unordered_map>
#include <memory>

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace gui
{
	typedef unsigned char(Volume);

	class AudioSystem
	{
	public:
		static const bool loadMusicFile(const unsigned short key, const std::string& path);
		static const bool loadSoundFile(const unsigned short key, const std::string& path);

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

#endif