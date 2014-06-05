#include "ChatServer.h"

ChatServer::ChatServer()
{

}

ChatServer::~ChatServer()
{
}

void ChatServer::update(const unsigned short & p_port = 2828)
{
	sf::TcpListener listener;

	listener.listen(p_port);

	while (true)
	{
		sf::TcpSocket client;

		if (listener.accept(client) == sf::Socket::Status::Done)
		{
			sf::Packet packet;
			packet << "Connected to chat server.\n";

			client.send(packet);
			std::cout << "Connection from: " << client.getRemoteAddress() << ":" << client.getRemotePort() << "\n";
		}
	}
}