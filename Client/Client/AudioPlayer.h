#pragma once
#include <SFML\Audio\Sound.hpp>
#include <SFML\Audio\SoundBuffer.hpp>
#include <SFML\Audio\Listener.hpp>
#include <SFML\System\Vector2.hpp>
#include <map>
#include <iostream>
#include <fstream>

class AudioPlayer{
public:
	AudioPlayer();
	~AudioPlayer();

	void update();

	void playSound(const std::string & p_sound, const sf::Vector3<float> & p_position);

private:
	std::map<std::string, sf::Sound> m_sounds;
	std::map<std::string, sf::SoundBuffer> m_buffers;
};