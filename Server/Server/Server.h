#pragma once
#include <SFML\Network.hpp>
#include <map>
#include <iostream>
#include <memory>

#include "..\..\Common\Protocol.h"
#include "..\..\Common\Map.h"
#include "GeneralMath.h"
#include "Client.h"

#include <SFML\System\Vector2.hpp>
#include <SFML\System\String.hpp>
#include <SFML\System\Clock.hpp>

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

	void pingClients();

	sf::Clock pingTimer;
};