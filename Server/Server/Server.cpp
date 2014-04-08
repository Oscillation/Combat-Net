#include "Server.h"

Client::Client(){}

Client::Client(const sf::IpAddress & p_address, const unsigned short & p_port) : m_address(p_address), m_port(p_port){
	
}

Client::~Client(){
	
}

sf::IpAddress Client::getAddress(){
	return m_address;
}

unsigned short Client::getPort(){
	return m_port;
}




Server::Server(const unsigned short & p_port) : m_port(p_port){
	
	run();
}

Server::~Server(){
	
}

void Server::run(){
	bool run = true;
	m_socket.setBlocking(false);
	m_socket.bind(m_port);
	while (run)
	{
		sf::IpAddress address;
		unsigned short port;
		sf::Packet packet;
		if (m_socket.receive(packet, address, port) == sf::Socket::Done)
		{
			sf::String name;
			packet >> name;
			m_clienList[name] = Client(address, port);
			std::cout << name.toAnsiString() << " has connected.\n";
		}
	}
}