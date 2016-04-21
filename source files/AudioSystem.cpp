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

#include "../include/GUI/AudioSystem.h"

#include <fstream>
#include <random>

namespace gui
{
	Volume                                                                    AudioSystem::musicVolume = 100, AudioSystem::soundVolume = 100, AudioSystem::masterVolume = 100;
	bool                                                                      AudioSystem::muted = false;
	unsigned short                                                            AudioSystem::currentSong = -1;
	std::unordered_map<unsigned short, std::unique_ptr<sf::Music>>            AudioSystem::music;
	std::unordered_map<unsigned short, std::pair<sf::Sound, sf::SoundBuffer>> AudioSystem::sound;

	const bool AudioSystem::loadMusicFile(const unsigned short key, const std::string& path)
	{
		if (key == unsigned short(-1))	return false;

		music[key] ? 0 : music.at(key).reset(new sf::Music());
		if (!music.at(key)->openFromFile(path))
		{
			music.erase(key);
			return false;
		}
		else return true;
	}

	const bool AudioSystem::loadSoundFile(const unsigned short key, const std::string& path)
	{
		if (!(sound.count(key) ? sound.at(key).second.loadFromFile(path) : sound[key].second.loadFromFile(path)))
		{
			sound.erase(key);
			return false;
		}
		else
		{
			sound.at(key).first.setBuffer(sound.at(key).second);
			return true;
		}
	}

	void AudioSystem::playSound(const unsigned short soundKey)
	{
		if (sound.count(soundKey))
		{
			sound.at(soundKey).first.setVolume(muted ? 0 : (soundVolume * masterVolume) / 100);
			sound.at(soundKey).first.play();
		}
	}

	void AudioSystem::playSong(const unsigned short key)
	{
		if (music.count(key))
		{
			if (music.count(currentSong))
			{
				if (!music.at(currentSong)) music.erase(currentSong);
				else if (music.at(currentSong)->getStatus() != sf::Music::Stopped) music.at(currentSong)->stop();
			}
			music.at(key)->setVolume(muted ? 0 : (musicVolume * masterVolume) / 100);
			music.at(key)->play();
			currentSong = key;
		}
	}

	void AudioSystem::playRandomSong()
	{
		if (music.empty()) return;

		std::random_device randomDevice;
		std::mt19937 mt(randomDevice());
		std::uniform_int_distribution<int> dist(0, music.size() - 1);
		std::unordered_map<unsigned short, std::unique_ptr<sf::Music>>::const_iterator it;

		if (music.count(currentSong))
		{
			if (music.at(currentSong)) music.at(currentSong)->stop();
			else music.erase(currentSong);
			do
			{
				it = music.begin();
				std::advance(it, dist(mt));
			} while (it->first == currentSong);
		}
		else
		{
			it = music.begin();
			std::advance(it, dist(mt));
		}

		currentSong = it->first;
		music.at(currentSong)->setVolume(muted ? 0 : (musicVolume * masterVolume) / 100);
		music.at(currentSong)->play();
	}

	void AudioSystem::setMusicVolume(const Volume newVolume)
	{
		newVolume < 100 ? musicVolume = newVolume : musicVolume = 100;
		if (music.count(currentSong))
			music.at(currentSong)->setVolume(muted ? 0 : (musicVolume * masterVolume) / 100);
	}

	void AudioSystem::setSoundVolume(const Volume newVolume)
	{
		newVolume < 100 ? soundVolume = newVolume : soundVolume = 100;
		for (auto it = sound.begin(), end = sound.end(); it != end; ++it)
			if (it->second.first.getStatus() == sf::Sound::Playing)
				it->second.first.setVolume(muted ? 0 : (masterVolume * soundVolume) / 100);
	}

	void AudioSystem::setMasterVolume(const Volume newVolume)
	{
		newVolume < 100 ? masterVolume = newVolume : masterVolume = 100;
		if (music.count(currentSong))
			music.at(currentSong)->setVolume(muted ? 0 : (musicVolume * masterVolume) / 100);
	}

	void AudioSystem::mute(const bool value)
	{
		muted = value;

		for (auto it = sound.begin(), end = sound.end(); it != end; ++it)
			if (it->second.first.getStatus() == sf::Sound::Playing)
				it->second.first.setVolume(muted ? 0 : (masterVolume * soundVolume) / 100);

		if (music.count(currentSong))
		{
			if (!music.at(currentSong))
			{
				music.erase(currentSong);
				return;
			}
			if (music.at(currentSong)->getStatus() == sf::Music::Playing)
				music.at(currentSong)->setVolume(muted ? 0 : (musicVolume * masterVolume) / 100);
		}
	}

	const Volume AudioSystem::getMusicVolume()
	{
		return musicVolume;
	}

	const Volume AudioSystem::getSoundVolume()
	{
		return soundVolume;
	}

	const Volume AudioSystem::getMasterVolume()
	{
		return masterVolume;
	}

	const bool AudioSystem::isSongPlaying()
	{
		return music.count(currentSong) && music.at(currentSong) && music.at(currentSong)->getStatus() == sf::Music::Playing;
	}

	const bool AudioSystem::isMuted()
	{
		return muted;
	}
}