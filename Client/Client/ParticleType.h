#pragma once
#include <SFML\System\Vector2.hpp>
#include <iostream>
#include <fstream>
#include <string>

struct ParticleType{
	ParticleType();
	ParticleType(const std::string & p_path);

	~ParticleType();

	///<summary>Seconds. x = min, y = max</summary>
	sf::Vector2<float> getTimeRange() const;

	///<summary>R-Value. x = min, y = max</summary>
	sf::Vector2<short> getRRange() const;

	///<summary>G-Value. x = min, y = max</summary>
	sf::Vector2<short> getGRange() const;

	///<summary>B-Value. x = min, y = max</summary>
	sf::Vector2<short> getBRange() const;

	///<summary>Pixels/second.</summary>
	float getSpeed() const;

	///<summary>Pixels/second. x = min, y = max</summary>
	sf::Vector2<short> getSpeedOffset() const;

private:
	sf::Vector2<float> m_timeRange;

	sf::Vector2<short> m_rRange;
	sf::Vector2<short> m_gRange;
	sf::Vector2<short> m_bRange;

	float m_speed;
	sf::Vector2<short> m_speedOffset;
};