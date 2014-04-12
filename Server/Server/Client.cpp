#include "Client.h"

Client::Client(){}

Client::Client(const sf::IpAddress & p_address, const unsigned short & p_port) : m_address(p_address), m_port(p_port){
	m_speed = 100;
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
