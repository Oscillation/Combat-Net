#include "ParticleType.h"

ParticleType::ParticleType(){

}

ParticleType::ParticleType(const std::string & p_path) :
	m_timeRange(sf::Vector2<float>(-1, -1))
{
	std::ifstream file;
	file.open(p_path);
	if (file.is_open())
	{
		std::string line;

		#pragma region Time
			{
				std::string minTime, maxTime;
				bool next = false;
				std::getline(file, line);
				for (int i = 0; i < line.length(); i++)
				{
					if (line[i] != ' ')
					{
						if (!next)
						{
							minTime.push_back(line[i]);
						}else
						{
							maxTime.push_back(line[i]);
						}
					}else
					{
						m_timeRange.x = std::atof(minTime.c_str());
						next = true;
					}
				}
				m_timeRange.y = std::atof(maxTime.c_str());
				line.clear();
			}
	#pragma endregion

		#pragma region Color
#pragma region R
		{
			std::string minR, maxR;
			bool next = false;
			std::getline(file, line);
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] != ' ')
				{
					if (!next)
					{
						minR.push_back(line[i]);
					}else
					{
						maxR.push_back(line[i]);
					}
				}else
				{
					m_rRange.x = std::atof(minR.c_str());
					next = true;
				}
			}
			m_rRange.y = std::atof(maxR.c_str());
			line.clear();
		}
#pragma endregion
#pragma region G
		{
			std::string minG, maxG;
			bool next = false;
			std::getline(file, line);
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] != ' ')
				{
					if (!next)
					{
						minG.push_back(line[i]);
					}else
					{
						maxG.push_back(line[i]);
					}
				}else
				{
					m_gRange.x = std::atof(minG.c_str());
					next = true;
				}
			}
			m_gRange.y = std::atof(maxG.c_str());
			line.clear();
		}
#pragma endregion
#pragma region B
		{
			std::string minB, maxB;
			bool next = false;
			std::getline(file, line);
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] != ' ')
				{
					if (!next)
					{
						minB.push_back(line[i]);
					}else
					{
						maxB.push_back(line[i]);
					}
				}else
				{
					m_bRange.x = std::atof(minB.c_str());
					next = true;
				}
			}
			m_bRange.y = std::atof(maxB.c_str());
			line.clear();
		}
#pragma endregion
#pragma endregion

		#pragma region Speed
		{
			std::string speed;
			std::getline(file, speed);
			m_speed = std::atof(speed.c_str());
		}
#pragma endregion

		#pragma region Speed offset
		{
			std::string minOffset, maxOffset;
			bool next = false;
			std::getline(file, line);
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] != ' ')
				{
					if (!next)
					{
						minOffset.push_back(line[i]);
					}else
					{
						maxOffset.push_back(line[i]);
					}
				}else
				{
					m_speedOffset.x = std::atof(minOffset.c_str());
					next = true;
				}
			}
			m_speedOffset.y = std::atof(maxOffset.c_str());
			line.clear();
		}
#pragma endregion

		file.close();
	}else
	{
		std::cout << "Unable to load particle from file: " << p_path << "\n";
	}
}

ParticleType::~ParticleType(){

}

sf::Vector2<float> ParticleType::getTimeRange() const{
	return m_timeRange;
}

sf::Vector2<short> ParticleType::getRRange() const{
	return m_rRange;
}

sf::Vector2<short> ParticleType::getGRange() const{
	return m_gRange;
}

sf::Vector2<short> ParticleType::getBRange() const{
	return m_bRange;
}

float ParticleType::getSpeed() const{
	return m_speed;
}

sf::Vector2<short> ParticleType::getSpeedOffset() const{
	return m_speedOffset;
}