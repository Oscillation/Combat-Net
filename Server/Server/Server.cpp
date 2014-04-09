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

//
//
//
//

Server::Server(const unsigned short & p_port) : m_port(p_port){
	while (m_socket.bind(m_port) != sf::Socket::Done)
	{
		std::cout << "Retry with port: ";
		std::cin >> m_port;
		sf::sleep(sf::seconds(1));
	}
	std::cout << "Server started.\nNow accepting connections to: " << sf::IpAddress::getPublicAddress() << ":" << m_port << ".\n\n\n\n";
	run();
}

Server::~Server(){

}

void Server::run(){
	bool run = true;
	m_socket.setBlocking(false);

	while (run)
	{
		sf::IpAddress address;
		unsigned short port;
		sf::Packet packet;
		if (m_socket.receive(packet, address, port) == sf::Socket::Done)
		{
			sf::String msg;
			int pt;
			packet >> pt;
			std::string from = "[" + address.toString() + ":" + std::to_string(port) + "]-" + std::to_string(pt) + ":";
			std::string str_msg = msg.toAnsiString();
			switch ((cn::PacketType)pt)
			{
			case cn::PlayerConnected:
				for (auto it = m_clienList.begin(); it != m_clienList.end(); ++it){
					m_socket.send(packet, it->second.getAddress(), it->second.getPort());
				}
				packet >> msg;
				m_clienList[msg.toAnsiString()] = Client(address, port);
				std::cout << from << msg.toAnsiString() << " has connected.\n";
				break;
			case cn::PlayerMessage:
				packet >> msg;
				std::cout << from << msg.toAnsiString() << "\n";
				break;
			default:
				packet >> msg;
				std::cout << from << "Unrecognized packet type: " << pt << ". Consult the protocol.\n" << msg.toAnsiString()<< "\n";
				break;
			}
		}
	}
}