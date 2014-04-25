#pragma once
#include "..\..\Common\Protocol.h"
#include <SFML\System\Time.hpp>
#include <SFML\System\String.hpp>
#include <SFML\Network\Packet.hpp>

struct InputData{
	InputData();
	InputData(const cn::InputType & p_type, const int & p_time, const int & p_deltaTime, const std::string & p_name);

	cn::InputType getInputType() const;
	int getTime() const;
	int getDeltaTime() const;

	///<summary>Return the player name.</summary>
	std::string getPlayer() const;

private:
	cn::InputType m_type;
	int m_time, m_deltaTime;
	std::string m_name;
};