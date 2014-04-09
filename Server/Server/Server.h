#pragma once
#include <SFML\Network.hpp>
#include <map>
#include <iostream>
#include <memory>

#include "..\..\Common\Protocol.h"
#include "..\..\Common\Map.h"
#include "GeneralMath.h"

#include <SFML\System\Vector2.hpp>
#include <SFML\System\String.hpp>

struct Client{
	Client();
	Client(const sf::IpAddress & p_address, const unsigned short & p_port);
	~Client();

	sf::IpAddress getAddress();

	unsigned short getPort();

	sf::Vector2f getPosition();

	void setPosition(const sf::Vector2f & p_position);

private:
	sf::IpAddress m_address;
	unsigned short m_port;

	sf::Vector2f m_position;
};

class Server{
public:
	Server(const unsigned short & p_port);
	~Server();

private:
	unsigned short m_port;

	sf::TcpListener m_listener;
	sf::UdpSocket m_socket;
	
	std::map<sf::String, Client> m_clientList;

	void run();
};