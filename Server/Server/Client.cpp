#include "Client.h"
#include <iostream>

Client::Client(){}

Client::Client(const sf::IpAddress & p_address, const unsigned short & p_port) : 
	m_address(p_address), 
	m_port(p_port),
	m_shootDelay(sf::seconds(0.3f)),
	m_speed(300),
	m_health(100),
	m_score(),
	m_damage(10)
{
	std::cout << "Called constructor for: " << p_address.toString() << ":" << p_port << "\n";
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

int Client::getHealth() const
{
	return m_health;
}

void Client::setHealth(const int& p_health)
{
	m_health = p_health;
}

void Client::damage(const int& p_damage)
{
	m_damage--;
	m_health -= p_damage;
	
}

int Client::getDamage() const
{
	return m_damage;
}

void Client::setDamage(const int& p_damage)
{
	m_damage = p_damage;
}

std::string Client::getName() const{
	return m_name;
}

bool Client::operator==(Client & p_client) const{
	return m_name == p_client.getName();
}