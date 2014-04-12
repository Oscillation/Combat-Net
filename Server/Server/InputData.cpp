#include "InputData.h"

InputData::InputData(){}

InputData::InputData(const cn::InputType & p_type, const int & p_time, const int & p_deltaTime, const sf::String & p_name) 
	: m_type(p_type),
	m_time(p_time),
	m_name(p_name),
	m_deltaTime(p_deltaTime)
{
	
}

cn::InputType InputData::getInputType() const{
	return m_type;
}

int InputData::getTime() const{
	return m_time;
}

int InputData::getDeltaTime() const{
	return m_deltaTime;
}

sf::String InputData::getPlayer() const{
	return m_name;
}