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
		sf::Packet retPacket;

		if (m_socket.receive(packet, address, port) == sf::Socket::Done)
		{
			std::string from = "[" + address.toString() + ":" + std::to_string(port) + "]: ";

			sf::String data;
			int pt;
			packet >> pt;

			switch ((cn::PacketType)pt)
			{
			case cn::PlayerConnected:
				
				packet >> data;
				
				m_clienList[data.toAnsiString()] = Client(address, port);

				retPacket << (int)cn::PlayerConnected << data;

				std::cout << from << data.toAnsiString() << " has connected.\n";
				break;

			case cn::PlayerDisconnected:
				packet >> data;
				m_clienList.erase(data);
				retPacket << cn::PlayerDisconnected << data;
				std::cout << from << data.toAnsiString() << " has disconnected." << std::endl;
				break;

			case cn::PlayerMessage:
				packet >> data;
				std::cout << from << data.toAnsiString() << "\n";
				break;

			default:
				packet >> data;
				std::cout << from << "Unrecognized packet type: " << pt << ". Consult the protocol.\n" << data.toAnsiString()<< "\n";
				break;
			}
		}
		for (auto it = m_clienList.begin(); it != m_clienList.end(); ++it){
			m_socket.send(retPacket, it->second.getAddress(), it->second.getPort());
		}
	}
}