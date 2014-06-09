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
	listener.setBlocking(false);

	std::vector<sf::TcpSocket*> clients;

	while (true)
	{
		sf::TcpSocket* client = new sf::TcpSocket();

		if (listener.accept(*client) == sf::Socket::Status::Done)
		{
			sf::Packet packet;
			packet << "Connected to chat server.\n";

			client->send(packet);
			std::cout << "Connection from: " << client->getRemoteAddress() << ":" << client->getRemotePort() << "\n";
			clients.push_back(client);
		}
	}

	for (int i = 0; i < clients.size(); i++)
	{
		delete clients[i];
	}

	clients.clear();
}