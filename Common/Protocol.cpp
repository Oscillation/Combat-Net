#include "Protocol.h"

sf::Packet& operator>>(sf::Packet & p_packet, cn::Score & p_score)
{
	p_packet >> p_score.points >> p_score.kills >> p_score.deaths;
	return p_packet;
}

sf::Packet& operator<<(sf::Packet & p_packet, cn::Score & p_score)
{
	p_packet << p_score.points << p_score.kills << p_score.deaths;
	return p_packet;
}