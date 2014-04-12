#include "Server.h"

Server::Server(const unsigned short & p_port) : m_port(p_port){
	while (m_socket.bind(m_port) != sf::Socket::Done)
	{
		std::cout << "Retry with port: ";
		std::cin >> m_port;
	}
	system("cls");
	std::cout << "Server started.\nNow accepting connections to: " << sf::IpAddress::getPublicAddress() << ":" << m_port << ".\n\n\n\n";
	pingTimer.restart();
	map = Map("Maps/map.txt");
	m_clock.restart();
	m_elapsed.restart();
	m_updateTime = sf::milliseconds(100);
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
			unsigned int pt, time;
			packet >> time >> pt;
			if (pt == cn::PlayerConnected)
			{
				playerConnected(packet, address, port);
			} if (pt == cn::PlayerDisconnected)
			{
				playerDisconnected(packet, address, port);
			}if (pt == cn::PlayerInput)
			{
				playerInput(packet, address, port, time);
			}
			if (pt == cn::Ping) 
			{
				sf::String name;
				packet >> name;
				m_clientList[name].hasRespondedToPing = true;
			} 
			/*if (pingTimer.getElapsedTime() > sf::seconds(1)){
			pingClients();
			pingTimer.restart();
			}*/
		}

		if (m_clock.getElapsedTime() > m_updateTime)
		{
			sf::Packet PACKET = simulateGameState();
			for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
				m_socket.send(PACKET, it->second.getAddress(), it->second.getPort());
				std::cout << "Sending mega packet to: " << it->first.toAnsiString() << std::endl;
			}
			m_clock.restart();
		}
	}
}

sf::Packet Server::simulateGameState() {
	sf::Packet retPacket;
	retPacket << m_elapsed.getElapsedTime().asMilliseconds() << cn::MegaPacket;
	for (InputData input : m_clientInputs) {

		Client* client = &m_clientList[input.getPlayer()];
		float deltaTime = (float)input.getDeltaTime()/1000.f;
		switch (input.getInputType())
		{
		case cn::MoveUp:
			client->move(0, -client->getSpeed()*deltaTime);
			break;

		case cn::MoveDown:
			client->move(0, client->getSpeed()*deltaTime);
			break;

		case cn::MoveLeft: 
			client->move(-client->getSpeed()*deltaTime, 0);
			break;

		case cn::MoveRight:
			client->move(client->getSpeed()*deltaTime, 0);
			break;

		case cn::ShootUp:
			break;

		case cn::ShootDown:
			break;

		case cn::ShootLeft: 
			break;

		case cn::ShootRight:
			break;

		default:
			break;
		}
	}
	retPacket << m_clientInputs.size();
	for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
		retPacket << cn::PlayerMove << it->first << it->second.getPosition().x << it->second.getPosition().y;
	}
	m_clientInputs.clear();
	return retPacket;
}

bool Server::nameTaken(const std::string & p_name){
	std::map<sf::String, Client>::const_iterator it = m_clientList.find(p_name);
	return it != m_clientList.end();
}

void Server::playerConnected(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & p_port){
	std::string from = "[" + p_address.toString() + ":" + std::to_string(p_port) + "]: ";
	sf::String data;
	sf::Packet retPacket;
	p_packet >> data;
	if (!nameTaken(data))
	{
		m_clientList[data] = Client(p_address, p_port);
		m_clientList[data].setPosition(sf::Vector2f(map.m_spawnPositions[(math::random(0, map.m_spawnPositions.size() - 1))].x + 25, map.m_spawnPositions[(math::random(0, map.m_spawnPositions.size() - 1))].y + 25));
		m_clientList[data].hasRespondedToPing = true;

		retPacket << m_elapsed.getElapsedTime().asMilliseconds() << (int)cn::PlayerConnected << data << m_clientList[data].getPosition().x << m_clientList[data].getPosition().y << map;
		m_socket.send(retPacket, p_address, p_port);

		//Send connecting client to already connected clients
		for (auto i = m_clientList.begin(); i != m_clientList.end(); i++)
		{
			if (i->first != data) {
				sf::Packet specialDelivery;
				specialDelivery << m_elapsed.getElapsedTime().asMilliseconds() << cn::PlayerConnected << data << m_clientList[data].getPosition().x << m_clientList[data].getPosition().y;
				m_socket.send(specialDelivery, i->second.getAddress(), i->second.getPort());
			}
		}

		//Send clients to connecting client
		for (auto i = m_clientList.begin(); i != m_clientList.end(); i++)
		{
			if (i->first != data) {
				sf::Packet specialDelivery;
				specialDelivery << m_elapsed.getElapsedTime().asMilliseconds() << cn::PlayerConnected << i->first << m_clientList[i->first].getPosition().x << m_clientList[i->first].getPosition().y;
				m_socket.send(specialDelivery, p_address, p_port);
			}
		}
		retPacket.clear();

		std::cout << from << data.toAnsiString() << " has connected. Sending map...\n";
	}else
	{
		retPacket << m_elapsed.getElapsedTime().asMilliseconds() << cn::NameTaken;
		m_socket.send(retPacket, p_address, p_port);
		std::cout << from << " attempted to connect with an already existing name...\n";
	}
}

void Server::playerDisconnected(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & p_port){
	std::string from = "[" + p_address.toString() + ":" + std::to_string(p_port) + "]: ";
	sf::String name;
	p_packet >> name;
	m_clientList.erase(name);
	sf::Packet packet;
	packet << m_elapsed.getElapsedTime().asMilliseconds() << cn::PlayerDisconnected << name;
	std::cout << from << name.toAnsiString() << " has disconnected." << std::endl;

	for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
		m_socket.send(packet, it->second.getAddress(), it->second.getPort());
	}
}

void Server::playerInput(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & port, const int & p_time){
	sf::String name;
	int length;

	p_packet >> name >> length;

	for (int i = 0; i < length; i++)
	{
		int inputType, deltaTime;
		p_packet >> inputType >> deltaTime;
		InputData data((cn::InputType)inputType, p_time, deltaTime, name);
		m_clientInputs.push_back(data);
	}
}

void Server::pingClients()
{
	// Check which clients are still connected
	for (auto it = m_clientList.begin(); it != m_clientList.end();){
		if (!it->second.hasRespondedToPing) {

			sf::Packet retPacket;
			retPacket << cn::PlayerDisconnected << it->first;

			for (auto it2 = m_clientList.begin(); it2 != m_clientList.end(); ++it2){
				m_socket.send(retPacket, it2->second.getAddress(), it2->second.getPort());
			}

			//m_clientList.erase(it++);
		} else {
			++it;
		}
	}

	// Ping clients
	sf::Packet pingPacket;
	pingPacket << cn::Ping;
	for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
		m_socket.send(pingPacket, it->second.getAddress(), it->second.getPort());
		it->second.hasRespondedToPing = false;
	}
}