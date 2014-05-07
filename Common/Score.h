#pragma once
#include <SFML\Network\Packet.hpp>

struct Score
{
	Score();
	int m_points;
	int m_deaths;
	int m_kills;
};

sf::Packet& operator>>(sf::Packet & p_packet, Score & p_score);

sf::Packet& operator<<(sf::Packet & p_packet, Score & p_score);