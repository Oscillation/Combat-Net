#pragma once
#include <SFML\Audio\Sound.hpp>
#include <SFML\Audio\SoundBuffer.hpp>
#include <map>
#include <iostream>

class AudioPlayer{
public:
	AudioPlayer();
	~AudioPlayer();

	void playSound(const std::string & p_sound);

private:
	sf::SoundBuffer m_soundBuffer;
	std::map<std::string, sf::Sound> m_sounds;
};