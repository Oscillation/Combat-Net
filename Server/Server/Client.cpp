#include "Client.h"

Client::Client(){}

Client::Client(const Circle & p_circle, const sf::IpAddress & p_address, const unsigned short & p_port) : m_circle(p_circle), m_address(p_address), m_port(p_port){
}

Client::~Client(){

}

sf::IpAddress Client::getAddress(){
	return m_address;
}

unsigned short Client::getPort(){
	return m_port;
}

sf::Vector2<float> Client::getPosition() const{
	return m_circle.getPosition();
}

Circle Client::getCircle() const{
	return m_circle;
}

void Client::setPosition(const sf::Vector2f & p_position){
	m_circle.setPosition(p_position);
}