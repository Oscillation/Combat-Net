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
			sound.setAttenuation(50.f);
			//sound.setRelativeToListener(true);

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

void AudioPlayer::update(){
	for (int i = 0; i < m_activeSounds.size(); i++)
	{
		if (m_activeSounds[i]->getStatus() == sf::Sound::Status::Stopped)
		{
			delete m_activeSounds[i];
			m_activeSounds.erase(m_activeSounds.begin() + i);
		}
	}
}

void AudioPlayer::playSound(const std::string & p_sound, const sf::Vector3<float> & p_position){
	sf::Sound* sound = new sf::Sound(m_sounds[p_sound]);

	sound->setPosition(p_position);

	m_activeSounds.push_back(sound);
	m_activeSounds.back()->play();

	/*m_sounds[p_sound].setPosition(p_position);
	m_sounds[p_sound].play();*/
}