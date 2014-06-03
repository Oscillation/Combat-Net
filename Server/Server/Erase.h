#pragma once
#include <SFML\System\Vector2.hpp>

struct Erase
{
	Erase();

	Erase(const int & p_id = -1, const sf::Vector2<float> & p_position = sf::Vector2<float>(0, 0), const bool & p_hitPlayer = false) :
		m_id(p_id),
		m_position(p_position),
		m_hitPlayer(p_hitPlayer)
	{
	
	}

	~Erase();

	int m_id;
	sf::Vector2<float> m_position;
	bool m_hitPlayer;
};