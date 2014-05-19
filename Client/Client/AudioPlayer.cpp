#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(){
	sf::SoundBuffer soundBuffer = sf::SoundBuffer();

	if (!soundBuffer.loadFromFile("Audio/projectile_hit_wall.wav"))
	{
		
	}

	sf::Sound sound = sf::Sound(soundBuffer);

	m_sounds["projectile_hit_wall"] = sound;
}

AudioPlayer::~AudioPlayer(){
	
}

void AudioPlayer::playSound(const std::string & p_sound){
	m_sounds[p_sound].play();
}