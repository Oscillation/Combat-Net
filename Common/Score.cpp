#include "Score.h"

Score::Score() : 
	m_points(0),
	m_deaths(0),
	m_kills(0)
{
	
}

sf::Packet& operator>>(sf::Packet & p_packet, Score & p_score)
{
	p_packet >> p_score.m_points >> p_score.m_kills >> p_score.m_deaths;
	return p_packet;
}

sf::Packet& operator<<(sf::Packet & p_packet, Score & p_score)
{
	p_packet << p_score.m_points << p_score.m_kills << p_score.m_deaths;
	return p_packet;
}