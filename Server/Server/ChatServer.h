#pragma once
#include <SFML\Network\TcpSocket.hpp>
#include <SFML\Network\TcpListener.hpp>
#include <SFML\Network\Packet.hpp>
#include <map>
#include <iostream>

#include "Client.h"

class ChatServer
{
public:
	ChatServer();
	~ChatServer();

	static void update(const unsigned short & p_port);
};