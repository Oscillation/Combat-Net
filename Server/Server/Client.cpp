#include "Client.h"

Client::Client(){}

Client::Client(const sf::IpAddress & p_address, const unsigned short & p_port) : 
	m_address(p_address), 
	m_port(p_port),
	m_shootDelay(sf::seconds(0.3f))
{
	m_speed = 300;
	score.points = 12;
	score.kills = 0;
	score.deaths = 0;
}

Client::~Client(){

}

sf::IpAddress Client::getAddress(){
	return m_address;
}

unsigned short Client::getPort(){
	return m_port;
}

void Client::setSpeed(float p_speed) {
	m_speed = p_speed;
}

float Client::getSpeed() const {
	return m_speed;
}

void Client::setName(const std::string & p_name){
	m_name = p_name;
}

bool Client::shoot() {
	if (m_lastShot.getElapsedTime() > m_shootDelay){
		m_lastShot.restart();
		return true;
	}
	return false;
}

std::string Client::getName() const{
	return m_name;
}