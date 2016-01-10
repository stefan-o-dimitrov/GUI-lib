#include "AudioSystem.h"
#include <fstream>
#include <random>

gui::AudioSystem::volume                                                  gui::AudioSystem::musicVolume, gui::AudioSystem::interfaceVolume, gui::AudioSystem::gameVolume, gui::AudioSystem::masterVolume;
unsigned short                                                            gui::AudioSystem::currentSong;
std::unordered_map<unsigned short, sf::Music*>                            gui::AudioSystem::music;
std::unordered_map<unsigned short, std::pair<sf::Sound, sf::SoundBuffer>> gui::AudioSystem::interfaceSound, gui::AudioSystem::gameSound;

void gui::AudioSystem::loadResources(const std::string& _musicPath, const std::string& _interfaceSoundPath, const std::string& _gameSoundPath)
{
	loadMusic(_musicPath);
	loadInterfaceSound(_interfaceSoundPath);
	loadGameSound(_gameSoundPath);
}

void gui::AudioSystem::loadMusic(const std::string& _path)
{
	std::ifstream file(_path);
	if (file.good())
	{
		while (!file.eof())
		{
			std::string buffer;

			std::getline(file, buffer, '"');
			std::getline(file, buffer, '"');
			const unsigned short KEY_VAL = atoi(buffer.c_str());
			music[KEY_VAL];

			std::getline(file, buffer, '"');
			std::getline(file, buffer, '"');
			music[KEY_VAL]->openFromFile(buffer);
		}
	}
}

void gui::AudioSystem::loadInterfaceSound(const std::string& _path)
{
	std::ifstream file(_path);
	if (file.good())
	{
		while (!file.eof())
		{
			std::string buffer;

			std::getline(file, buffer, '"');
			std::getline(file, buffer, '"');
			const unsigned short KEY_VAL = atoi(buffer.c_str());
			interfaceSound[KEY_VAL];

			std::getline(file, buffer, '"');
			std::getline(file, buffer, '"');
			interfaceSound[KEY_VAL].second.loadFromFile(buffer);
			interfaceSound[KEY_VAL].first.setBuffer(interfaceSound[KEY_VAL].second);
		}
	}
}

void gui::AudioSystem::loadGameSound(const std::string& _path)
{
	std::ifstream file(_path);
	if (file.good())
	{
		while (!file.eof())
		{
			std::string buffer;

			std::getline(file, buffer, '"');
			std::getline(file, buffer, '"');
			const unsigned short KEY_VAL = atoi(buffer.c_str());
			gameSound[KEY_VAL];

			std::getline(file, buffer, '"');
			std::getline(file, buffer, '"');
			gameSound[KEY_VAL].second.loadFromFile(buffer);
			gameSound[KEY_VAL].first.setBuffer(interfaceSound[KEY_VAL].second);
		}
	}
}

void gui::AudioSystem::playInterfaceSound(const unsigned short _soundKey)
{
	if (interfaceSound.count(_soundKey))
	{
		interfaceSound[_soundKey].first.setVolume((musicVolume * masterVolume) / 100);
		interfaceSound[_soundKey].first.play();
	}
}

void gui::AudioSystem::playGameSound(const unsigned short _soundKey)
{
	if (gameSound.count(_soundKey))
	{
		gameSound[_soundKey].first.setVolume((musicVolume * masterVolume) / 100);
		gameSound[_soundKey].first.play();
	}
}

void gui::AudioSystem::playSong(const unsigned short _song)
{
	if (music.count(_song))
	{
		if (music.count(currentSong) && music[currentSong]->getStatus() != sf::SoundStream::Stopped) music[currentSong]->stop();
		music[_song]->play();
		currentSong = _song;
	}
}

void gui::AudioSystem::playRandomSong()
{
	if ((music.count(currentSong) && music[currentSong]->getStatus() == sf::SoundStream::Stopped) || !music.count(currentSong))
	{
		std::random_device randomDevice;
		std::mt19937 mt(randomDevice());
		std::uniform_int_distribution<int> dist(0, music.size() - 1);

		unsigned short buffer;
		std::unordered_map<unsigned short, sf::Music*>::const_iterator it;
		do
		{ 
			it = music.begin();
			buffer = dist(mt);
			std::advance(it, buffer);
		} while (it->first == currentSong);

		currentSong = it->first;
		music[currentSong]->setVolume((musicVolume * masterVolume) / 100);
		music[currentSong]->play();
	}
}

void gui::AudioSystem::setMusicVolume(const volume _volume)
{
	musicVolume = _volume;
}

void gui::AudioSystem::setInterfaceVolume(const volume _volume)
{
	interfaceVolume = _volume;
}

void gui::AudioSystem::setSoundVolume(const volume _volume)
{
	gameVolume = _volume;
}

void gui::AudioSystem::setMasterVolume(const volume _volume)
{
	masterVolume = _volume;
}

const gui::AudioSystem::volume gui::AudioSystem::getMusicVolume()
{
	return musicVolume;
}

const gui::AudioSystem::volume gui::AudioSystem::getInterfaceVolume()
{
	return interfaceVolume;
}

const gui::AudioSystem::volume gui::AudioSystem::getGameVolume()
{
	return gameVolume;
}

const gui::AudioSystem::volume gui::AudioSystem::getMasterVolume()
{
	return masterVolume;
}

const sf::Sound& gui::AudioSystem::getInterfaceSound(const unsigned short _soundKey)
{
	return interfaceSound.at(_soundKey).first;
}

const sf::Sound& gui::AudioSystem::getGameSound(const unsigned short _soundKey)
{
	return gameSound.at(_soundKey).first;
}