#pragma once
#include <SFML\Network\TcpSocket.hpp>
#include <SFML\Network\TcpListener.hpp>
#include <SFML\Network\Packet.hpp>
#include <map>

#include "Client.h"

class ChatServer
{
public:
	ChatServer(const unsigned short & p_port = 2828);
	~ChatServer();

	void update();

private:
	sf::TcpListener m_listener;

	unsigned short m_port;
};