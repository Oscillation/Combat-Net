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
	run();

}

Server::~Server(){

}

void Server::run(){
	bool run = true;
	m_socket.setBlocking(false);

	Map map = Map("Maps/map.txt");
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
				if (!nameTaken(data))
				{
					m_clientList[data.toAnsiString()] = Client(address, port);
					m_clientList[data.toAnsiString()].setPosition(sf::Vector2f(map.m_spawnPositions[(math::random(0, map.m_spawnPositions.size() - 1))].x + 25, map.m_spawnPositions[(math::random(0, map.m_spawnPositions.size() - 1))].y + 25));
					m_clientList[data.toAnsiString()].hasRespondedToPing = true;

					retPacket << (int)cn::PlayerConnected << data << m_clientList[data.toAnsiString()].getPosition().x << m_clientList[data.toAnsiString()].getPosition().y;

					m_socket.send(retPacket, address, port);

					retPacket.clear();
					retPacket << cn::Map << map;
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
					retPacket.clear();

					std::cout << from << data.toAnsiString() << " has connected. Sending map...\n";
				}else
				{
					retPacket << cn::NameTaken;
					m_socket.send(retPacket, address, port);
					std::cout << from << " attempted to connect with an already existing name...\n";
				}
			} if (pt == cn::PlayerDisconnected)
			{
				packet >> data;
				m_clientList.erase(data);
				retPacket << cn::PlayerDisconnected << data;
				shouldSend = true;
				std::cout << from << data.toAnsiString() << " has disconnected." << std::endl;
			} if (pt == cn::PlayerMessage)
			{
				packet >> data;
				std::cout << from << data.toAnsiString() << "\n";
			} if (pt == cn::PlayerInput)
			{
				short input;
				int inputCount;
				bool move = false;
				sf::Vector2<float> vel;
				
				packet >> data >> inputCount;
				m_clientList[data.toAnsiString()].setPosition(m_clientList[data.toAnsiString()].getPosition());

				for (int i = 0; i < inputCount; i++)
				{
					packet >> input;
					switch (input)
					{
					case 0:
						vel.y -= 3;
						move = true;
						break;
					case 1:
						vel.y += 3;
						move = true;
						break;
					case 2:
						vel.x -= 3;
						move = true;
						break;
					case 3:
						vel.x += 3;
						move = true;
						break;
					default:
						break;
					}
					/*if (input == 4)
					{
						std::unique_ptr<Projectile> projectile(new Projectile());
						projectile->setPosition(m_clientList[data.toAnsiString()].getPosition());
						m_projectileList[data.toAnsiString()].push_back(std::move(projectile));
						retPacket << cn::Projectile << data << m_projectileList[data.toAnsiString()].back();
					}
					if (input == 5)
					{
						std::unique_ptr<Projectile> projectile(new Projectile());
						projectile->setPosition(m_clientList[data.toAnsiString()].getPosition());
						m_projectileList[data.toAnsiString()].push_back(std::move(projectile));
						retPacket << cn::Projectile << data << m_projectileList[data.toAnsiString()].back();
					}
					if (input == 6)
					{
						std::unique_ptr<Projectile> projectile(new Projectile());
						projectile->setPosition(m_clientList[data.toAnsiString()].getPosition());
						m_projectileList[data.toAnsiString()].push_back(std::move(projectile));
						retPacket << cn::Projectile << data << m_projectileList[data.toAnsiString()].back();
					}
					if (input == 7)
					{
						std::unique_ptr<Projectile> projectile(new Projectile());
						projectile->setPosition(m_clientList[data.toAnsiString()].getPosition());
						m_projectileList[data.toAnsiString()].push_back(std::move(projectile));
						retPacket << cn::Projectile << data << m_projectileList[data.toAnsiString()].back();
					}*/
					
					if (!map.intersectsWall(sf::Vector2<float>(m_clientList[data.toAnsiString()].getPosition().x, m_clientList[data.toAnsiString()].getPosition().y + vel.y)))
					{
						m_clientList[data.toAnsiString()].setPosition(sf::Vector2<float>(m_clientList[data.toAnsiString()].getPosition().x, m_clientList[data.toAnsiString()].getPosition().y + vel.y));
					}else
					{
						//resolve
					}
					if (!map.intersectsWall(sf::Vector2<float>(m_clientList[data.toAnsiString()].getPosition().x + vel.x, m_clientList[data.toAnsiString()].getPosition().y)))
					{
						m_clientList[data.toAnsiString()].setPosition(sf::Vector2<float>(m_clientList[data.toAnsiString()].getPosition().x + vel.x, m_clientList[data.toAnsiString()].getPosition().y));
					}else
					{
						//resolve
					}
				}
				if (move)
				{
					retPacket << cn::PlayerMove << data << m_clientList[data.toAnsiString()].getPosition().x << m_clientList[data.toAnsiString()].getPosition().y;
				}
				shouldSend = true;
			} if (pt == cn::Ping) 
			{
				sf::String name;
				packet >> name;
				m_clientList[name].hasRespondedToPing = true;
			} 
			//for (auto it = m_projectileList.begin(); it != m_projectileList.end(); ++it){
			//	for (unsigned int i = 0; i < it->second.size(); i++)
			//	{
			//		//it->second[i].get()->move(it->second[i].get()->getVelocity());
			//		retPacket << cn::Projectile << it->first << it->second[i];
			//		m_socket.send(retPacket, address, port);
			//	}
			//}
		}
		if (shouldSend)
		{
			for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
				m_socket.send(retPacket, it->second.getAddress(), it->second.getPort());
			}
		}
		retPacket.clear();
		shouldSend = false;

		if (pingTimer.getElapsedTime() > sf::seconds(1)){
			pingClients();
			pingTimer.restart();
		}
	}
}

bool Server::nameTaken(const std::string & p_name){
	std::map<sf::String, Client>::const_iterator it = m_clientList.find(p_name);
	return it != m_clientList.end();
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

			m_clientList.erase(it++);
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