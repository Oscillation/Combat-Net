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

sf::Vector2f Client::getPosition(){
	return m_position;
}

void Client::setPosition(const sf::Vector2f & p_position){
	m_position = p_position;
}



Server::Server(const unsigned short & p_port) : m_port(p_port){
	while (m_socket.bind(m_port) != sf::Socket::Done)
	{
		std::cout << "Retry with port: ";
		std::cin >> m_port;
	}
	system("cls");
	std::cout << "Server started.\nNow accepting connections to: " << sf::IpAddress::getPublicAddress() << ":" << m_port << ".\n\n\n\n";
	run();
}

Server::~Server(){

}

void Server::run(){
	bool run = true;
	m_socket.setBlocking(false);

	Map* map = new Map();
	map->m_tiles[0][0].m_type = Wall;
	map->m_tiles[1][0].m_type = Wall;
	while (run)
	{
		sf::IpAddress address;
		unsigned short port;
		sf::Packet packet;
		sf::Packet retPacket;

		bool shouldSend = false;

		if (m_socket.receive(packet, address, port) == sf::Socket::Done)
		{
			std::string from = "[" + address.toString() + ":" + std::to_string(port) + "]: ";
			sf::String data;

			int pt;
			packet >> pt;
			if (pt == cn::PlayerConnected)
			{
				packet >> data;

				m_clientList[data.toAnsiString()] = Client(address, port);
				m_clientList[data.toAnsiString()].setPosition(sf::Vector2f((float)math::random(0, 600),  (float)math::random(0, 400)));

				retPacket << (int)cn::PlayerConnected << data << m_clientList[data.toAnsiString()].getPosition().x << m_clientList[data.toAnsiString()].getPosition().y;
				
				m_socket.send(retPacket, address, port);

				retPacket.clear();
				retPacket << cn::Map << *map;
				m_socket.send(retPacket, address, port);

				for (auto i = m_clientList.begin(); i != m_clientList.end(); i++)
				{
					if (i->first != data) {
						sf::Packet specialDelivery;
						specialDelivery << cn::PlayerConnected << data << m_clientList[data.toAnsiString()].getPosition().x << m_clientList[data.toAnsiString()].getPosition().y;
						m_socket.send(specialDelivery, i->second.getAddress(), i->second.getPort());
					}
				}

				for (auto i = m_clientList.begin(); i != m_clientList.end(); i++)
				{
					if (i->first != data) {
						sf::Packet specialDelivery;
						specialDelivery << cn::PlayerConnected << i->first << m_clientList[i->first.toAnsiString()].getPosition().x << m_clientList[i->first.toAnsiString()].getPosition().y;
						m_socket.send(specialDelivery, address, port);
					}
				}

				
				shouldSend = true;
				std::cout << from << data.toAnsiString() << " has connected. Sending map...\n";
			}else if (pt == cn::PlayerDisconnected)
			{
				packet >> data;
				m_clientList.erase(data);
				retPacket << cn::PlayerDisconnected << data;
				shouldSend = true;
				std::cout << from << data.toAnsiString() << " has disconnected." << std::endl;
			}else if (pt == cn::PlayerMessage)
			{
				packet >> data;
				std::cout << from << data.toAnsiString() << "\n";
			}else if (pt == cn::PlayerInput)
			{
				int input;
				int inputCount;
				packet >> data >> inputCount;
				sf::Vector2f pos = m_clientList[data.toAnsiString()].getPosition();
				for (int i = 0; i < inputCount; i++)
				{
					packet >> input;
					switch (input)
					{
					case 0:
						pos.y -= 10;
						break;
					case 1:
						pos.y += 10;
						break;
					case 2:
						pos.x -= 10;
						break;
					case 3:
						pos.x += 10;
						break;
					default:
						break;
					}
				}
				m_clientList[data.toAnsiString()].setPosition(pos);

				retPacket << cn::PlayerMove << data << pos.x << pos.y;
				shouldSend = true;
			}else
			{
				packet >> data;
				std::cout << from << "Unrecognized packet type: " << pt << ". Consult the protocol.\n" << data.toAnsiString()<< "\n";
			}

		}
		if (shouldSend)
		{
			for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
				m_socket.send(retPacket, it->second.getAddress(), it->second.getPort());
			}
		}
		retPacket.clear();
		shouldSend = false;
	}
}