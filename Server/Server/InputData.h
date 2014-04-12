#pragma once
#include "..\..\Common\Protocol.h"
#include <SFML\System\Time.hpp>
#include <SFML\System\String.hpp>
#include <SFML\Network\Packet.hpp>

struct InputData{
	InputData();
	InputData(const cn::InputType & p_type, const int & p_time, const int & p_deltaTime, const sf::String & p_name);

	cn::InputType getInputType() const;
	int getTime() const;
	int getDeltaTime() const;

	///<summary>Return the player name.</summary>
	sf::String getPlayer() const;

private:
	cn::InputType m_type;
	int m_time, m_deltaTime;
	sf::String m_name;
};