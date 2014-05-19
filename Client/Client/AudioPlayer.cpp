#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(){
	

	if (!m_soundBuffer.loadFromFile("Audio/projectile_hit_wall.wav"))
	{
		std::cout << "failed to load wav file.\n";
	}

	sf::Sound sound = sf::Sound(m_soundBuffer);

	m_sounds["projectile_hit_wall"] = sound;
}

AudioPlayer::~AudioPlayer(){
	
}

void AudioPlayer::playSound(const std::string & p_sound){
	m_sounds[p_sound].play();
}