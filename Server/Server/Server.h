#pragma once
#include <SFML\Network.hpp>
#include <map>
#include <iostream>
#include "..\..\Client\Client\Protocol.h"

#include <SFML\System\String.hpp>

struct Client{
	Client();
	Client(const sf::IpAddress & p_address, const unsigned short & p_port);
	~Client();

	sf::IpAddress getAddress();

	unsigned short getPort();

private:
	sf::IpAddress m_address;
	unsigned short m_port;
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