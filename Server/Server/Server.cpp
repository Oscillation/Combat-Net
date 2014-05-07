#include "Server.h"

Server::Server(const unsigned short & p_port) : m_port(p_port), m_projectileID(0){
	while (m_socket.bind(m_port) != sf::Socket::Done)
	{
		std::cout << "Retry with port: ";
		std::cin >> m_port;
	}
	system("cls");
	std::cout << "Server started.\nNow accepting connections to: " << sf::IpAddress::getPublicAddress() << ":" << m_port << ".\n\n\n\n";

	map = Map("Maps/amazing map.txt");
	m_gameManager = GameManager(&m_clientList, &m_projectiles, map.m_tiles.size(), map.m_tiles.begin()->size());

	pingTimer.restart();
	m_clock.restart();
	m_elapsed.restart();
	m_updateTime = sf::milliseconds(50);

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
			}
			if (pt == cn::PlayerDisconnected)
			{
				playerDisconnected(packet, address, port);
			}
			if (pt == cn::PlayerInput)
			{
				playerInput(packet, address, port, time);
			}
			if (pt == cn::Projectile)
			{
				sf::Packet projectilePacket = projectile(packet, address, port, time);
				for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
					m_socket.send(projectilePacket, it->second.getAddress(), it->second.getPort());
				}
			}
			if (pt == cn::Ping) 
			{
				std::string name;
				packet >> name;
				m_clientList[name].hasRespondedToPing = true;
			} 
		}

		if (pingTimer.getElapsedTime() > sf::seconds(1)){
			pingClients();
			pingTimer.restart();
		}

		if (m_clock.getElapsedTime() > m_updateTime)
		{
			retPacket = simulateGameState();
			for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
				m_socket.send(retPacket, it->second.getAddress(), it->second.getPort());
			}
			m_clock.restart();
		}
	}
}

std::vector<Projectile>::iterator Server::findID(const int & p_id){
	for (std::vector<Projectile>::iterator it = m_projectiles.begin(); it != m_projectiles.end(); ++it){
		if (it->m_id == p_id)
		{
			if (it != m_projectiles.end())
			{
				return it;
			}else
			{
				std::cout << "Searched for invalid projectile id: " << p_id << "\n";
			}
		}
	}
	std::cout << "Searched for invalid projectile id: " << p_id << "\n";
}

sf::Packet Server::simulateGameState() {
	sf::Packet retPacket;
	retPacket << m_elapsed.getElapsedTime().asMilliseconds() << cn::MegaPacket;
	for (InputData input : m_clientInputs) {
		Client* client = &m_clientList[input.getPlayer()];
		float deltaTime = (float)input.getDeltaTime()/1000.f;
		sf::Vector2<float> pos = client->getPosition();
		switch (input.getInputType())
		{
		case cn::MoveUp:
			if (!map.intersectsWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y -client->getSpeed()*deltaTime)))
			{
				client->move(0, -client->getSpeed()*deltaTime);
			}else
			{
				client->setPosition(client->getPosition().x, map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y -client->getSpeed()*deltaTime)).y + 64 + 20);
			}
			break;

		case cn::MoveDown:
			if (!map.intersectsWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y + client->getSpeed()*deltaTime)))
			{
				client->move(0, client->getSpeed()*deltaTime);
			}else
			{
				client->setPosition(client->getPosition().x, map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y + client->getSpeed()*deltaTime)).y - 20);
			}
			break;

		case cn::MoveLeft:
			if (!map.intersectsWall(sf::Vector2<float>(client->getPosition().x -client->getSpeed()*deltaTime, client->getPosition().y)))
			{
				client->move(-client->getSpeed()*deltaTime, 0);
			}else
			{
				client->setPosition(map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x -client->getSpeed()*deltaTime, client->getPosition().y)).x + 64 + 20, client->getPosition().y);
			}
			break;

		case cn::MoveRight:
			if (!map.intersectsWall(sf::Vector2<float>(client->getPosition().x + client->getSpeed()*deltaTime, client->getPosition().y)))
			{
				client->move(client->getSpeed()*deltaTime, 0);
			}else
			{
				client->setPosition(map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x + client->getSpeed()*deltaTime, client->getPosition().y)).x - 20, client->getPosition().y);
			}
			break;
		}

		sf::Vector2<int> clientPoints[4] = {sf::Vector2<int>(pos.x/(64*m_gameManager.m_size), pos.y/(64*m_gameManager.m_size)), sf::Vector2<int>((pos.x + 40)/(64*m_gameManager.m_size), pos.y/(64*m_gameManager.m_size)), sf::Vector2<int>(pos.x/(64*m_gameManager.m_size), (pos.y + 40)/(64*m_gameManager.m_size)), sf::Vector2<int>((pos.x + 40)/(64*m_gameManager.m_size), (pos.y + 40)/(64*m_gameManager.m_size))};

		for (int i = 0; i < 4; i++)
		{
			m_gameManager.m_branches[clientPoints[i].x+((clientPoints[i].y)*m_gameManager.m_mapSize.x)].update(*client);
		}
	}

	m_clientInputs.clear();

	for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
		m_gameManager.update(it->second);
		retPacket << cn::PlayerMove << it->second.getName() << it->second.getPosition().x << it->second.getPosition().y;
	}

	m_eraseProjectileIDs.clear();


	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); ++it){
		if (!it->erase)
		{
			it->move(it->getVelocity());
			it->erase = map.intersectsWall(it->getPosition());
			if (it->erase)
			{
				m_eraseProjectileIDs.push_back(it->m_id);
			}else
			{
				m_gameManager.update(*it);
				std::vector<Client> clients = m_gameManager.getClients(*it);
				// itER = playER, it ends with an "er" deuuuug
				for (auto iter = clients.begin(); iter != clients.end(); ++iter){
					if (m_gameManager.intersect(*iter, *it))
					{
						//if (iter->getName() != it->getName())
						{
							std::cout << iter->getHealth() << "\n";
							iter->damage(it->m_damage);
							retPacket << cn::PlayerHealth << iter->getName() << iter->getHealth();
							it->erase = true;
							m_eraseProjectileIDs.push_back(it->m_id);
						}
					}
				}
			}
		}
	}

	if (!m_eraseProjectileIDs.empty())
	{
		retPacket << cn::EraseProjectile << m_eraseProjectileIDs.size();
		for (auto it = m_eraseProjectileIDs.begin(); it != m_eraseProjectileIDs.end(); ++it){
			retPacket << *it;
			std::vector<Projectile>::iterator iter = findID(*it);
			if (iter != m_projectiles.end())
			{
				m_gameManager.erase(*iter);
				m_projectiles.erase(iter);
			}
		}
		m_eraseProjectileIDs.clear();
	}

	if (!m_projectiles.empty())
	{
		retPacket << cn::Projectile << m_projectiles;
	}

	for (auto i = m_clientList.begin(); i != m_clientList.end(); i++)
	{
		retPacket << cn::ScoreUpdate << i->second.getName() << i->second.m_score;
	}

	return retPacket;
}

bool Server::nameTaken(const std::string & p_name){
	std::map<std::string, Client>::const_iterator it = m_clientList.find(p_name);
	return it != m_clientList.end();
}

void Server::playerConnected(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & p_port){
	std::string from = "[" + p_address.toString() + ":" + std::to_string(p_port) + "]: ";
	std::string data;
	sf::Packet retPacket;
	p_packet >> data;
	if (!nameTaken(data))
	{
		m_clientList[data] = Client(p_address, p_port);
		m_clientList[data].setName(data);
		m_clientList[data].setPosition(sf::Vector2f(map.m_spawnPositions[(math::random(0, map.m_spawnPositions.size() - 1))].x + 25, map.m_spawnPositions[(math::random(0, map.m_spawnPositions.size() - 1))].y + 25));
		m_clientList[data].hasRespondedToPing = true;

		retPacket << m_elapsed.getElapsedTime().asMilliseconds() << (int)cn::PlayerConnected << data << m_clientList[data].getPosition().x << m_clientList[data].getPosition().y << map << m_projectiles;
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

		std::cout << from << data << " has connected. Sending map...\n";
	}else
	{
		retPacket << m_elapsed.getElapsedTime().asMilliseconds() << cn::NameTaken;
		m_socket.send(retPacket, p_address, p_port);
		std::cout << from << " attempted to connect with an already existing name...\n";
	}
}

void Server::playerDisconnected(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & p_port){
	std::string from = "[" + p_address.toString() + ":" + std::to_string(p_port) + "]: ";
	std::string name;
	p_packet >> name;
	m_gameManager.erase(m_clientList[name]);
	m_clientList.erase(name);
	sf::Packet packet;
	packet << m_elapsed.getElapsedTime().asMilliseconds() << cn::PlayerDisconnected << name;
	std::cout << from << name << " has disconnected." << std::endl;

	for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
		m_socket.send(packet, it->second.getAddress(), it->second.getPort());
	}
}

void Server::playerInput(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & port, const int & p_time){
	std::string name;
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

sf::Packet Server::projectile(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & port, const int & p_time){
	sf::Packet retPacket;
	retPacket << 0 << cn::Projectile;
	if (m_projectiles.empty())
	{
		m_projectileID = 0;
	}

	int length;
	std::vector<Projectile> projectiles;
	p_packet >> projectiles;

	for (auto it = projectiles.begin(); it != projectiles.end();) {
		if (m_clientList[projectiles.begin()->getName()].shoot()) {
			if (it->m_id == -1) {
				it->m_id = m_projectileID;
				it->m_damage = m_clientList[projectiles.begin()->getName()].getDamage();
				m_projectileID++;
			}

			m_projectiles.push_back(*it);
			it++;
		} else {
			it = projectiles.erase(it);
		}
	}
		
	retPacket << projectiles;
	return retPacket;
}

void Server::pingClients()
{
	// Check which clients are still connected
	for (auto it = m_clientList.begin(); it != m_clientList.end();){
		if (!it->second.hasRespondedToPing) {

			sf::Packet retPacket;
			retPacket << m_clock.getElapsedTime().asMilliseconds() << cn::PlayerDisconnected << it->first;

			for (auto it2 = m_clientList.begin(); it2 != m_clientList.end(); ++it2){
				m_socket.send(retPacket, it2->second.getAddress(), it2->second.getPort());
			}
			std::cout << it->first << " has disconnected" << std::endl;
			m_clientList.erase(it++);

		} else {
			++it;
		}
	}

	// Ping clients
	sf::Packet pingPacket;
	pingPacket << m_clock.getElapsedTime().asMilliseconds() << cn::Ping;
	for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
		m_socket.send(pingPacket, it->second.getAddress(), it->second.getPort());
		it->second.hasRespondedToPing = false;
	}
}