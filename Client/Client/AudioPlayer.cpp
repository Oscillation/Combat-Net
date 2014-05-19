#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(){
	std::ifstream file;

	file.open("Audio/audio files.txt");

	if (file.is_open())
	{
		std::string line;
		
		while (!file.eof())
		{
			std::getline(file, line);

			auto pos = line.find(' ');

			std::string key = line.substr(0, pos), path = line.substr(pos + 1, line.size());

			sf::SoundBuffer soundBuffer;

			m_buffers[key] = soundBuffer;

			if (!m_buffers[key].loadFromFile("Audio/" + path))
			{
				std::cout << "Failed to load audio file: " << "Audio/" + path << "\n";
			}

			sf::Sound sound = sf::Sound(m_buffers[key]);
			
			m_sounds[key] = sound;
		}

		file.close();
	}else
	{
		std::cout << "Failed to open file: Audio/audio files.txt";
	}
}

AudioPlayer::~AudioPlayer(){
	
}

void AudioPlayer::playSound(const std::string & p_sound){
	m_sounds[p_sound].play();
}