#include "ChatServer.h"

ChatServer::ChatServer(const unsigned short & p_port) :
	m_port(p_port)
{
	m_listener.listen(m_port);
}

ChatServer::~ChatServer()
{
}

void ChatServer::update()
{
	sf::TcpSocket client;

	if (m_listener.accept(client) == sf::Socket::Status::Done)
	{
		
	}
}