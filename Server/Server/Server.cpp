#include "Server.h"

Server::Server(const unsigned short & p_port) : m_port(p_port), m_projectileID(0), m_speedBoost(5){
	while (m_socket.bind(m_port) != sf::Socket::Done)
	{
		std::cout << "Retry with port: ";
		std::cin >> m_port;
	}
	system("cls");
	std::cout << "Server started.\nNow accepting connections to: " << sf::IpAddress::getPublicAddress() << ":" << m_port << ".\n\n\n\n";

	m_map = Map("Maps/map.txt");
	m_gameManager = GameManager(&m_clientList, &m_projectiles, m_map.m_tiles.size(), m_map.m_tiles.begin()->size());
	m_powerManager = PowerManager(m_map.getPowerTiles());

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

		//std::cout << m_powerManager.m_powers.size() << "\n";

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

	m_powerManager.update(m_clock.getElapsedTime());

#pragma region Player movement
	for (InputData input : m_clientInputs) {
		Client* client = &m_clientList[input.getPlayer()];
		if (client->getHealth() > 0) {
			float deltaTime = (float)input.getDeltaTime()/1000.f;
			sf::Vector2<float> pos = client->getPosition();
			switch (input.getInputType())
			{
			case cn::MoveUp:
				if (!m_map.intersectsWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y -client->getSpeed()*deltaTime)))
				{
					client->move(0, (-client->getSpeed()*deltaTime) - (client->m_speedBoost*m_speedBoost*deltaTime));
				}else
				{
					client->setPosition(client->getPosition().x, m_map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y -client->getSpeed()*deltaTime)).y + 64 + 20);
				}
				break;

			case cn::MoveDown:
				if (!m_map.intersectsWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y + client->getSpeed()*deltaTime)))
				{
					client->move(0, client->getSpeed()*deltaTime + (client->m_speedBoost*m_speedBoost*deltaTime));
				}else
				{
					client->setPosition(client->getPosition().x, m_map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y + client->getSpeed()*deltaTime)).y - 20);
				}
				break;

			case cn::MoveLeft:
				if (!m_map.intersectsWall(sf::Vector2<float>(client->getPosition().x -client->getSpeed()*deltaTime, client->getPosition().y)))
				{
					client->move(-client->getSpeed()*deltaTime - (client->m_speedBoost*m_speedBoost*deltaTime), 0);
				}else
				{
					client->setPosition(m_map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x -client->getSpeed()*deltaTime, client->getPosition().y)).x + 64 + 20, client->getPosition().y);
				}
				break;

			case cn::MoveRight:
				if (!m_map.intersectsWall(sf::Vector2<float>(client->getPosition().x + client->getSpeed()*deltaTime, client->getPosition().y)))
				{
					client->move(client->getSpeed()*deltaTime + (client->m_speedBoost*m_speedBoost*deltaTime), 0);
				}else
				{
					client->setPosition(m_map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x + client->getSpeed()*deltaTime, client->getPosition().y)).x - 20, client->getPosition().y);
				}
				break;
			}
		}
	}
#pragma endregion

#pragma region Clean
	m_clientInputs.clear();
	m_eraseProjectileIDs.clear();
	m_gameManager.clean();
#pragma endregion

#pragma region Send player positions
	for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
		if (!m_powerManager.m_powers.empty() && it->second.getHealth() > 0)
		{
			for (int i = 0; i < m_powerManager.m_powers.size(); i++){
				if (math::circleIntersectsRect(sf::Vector2<float>(it->second.getPosition().x - 20, it->second.getPosition().y - 20), 20.f, sf::Rect<float>(m_powerManager.m_powers[i].getPosition().x, m_powerManager.m_powers[i].getPosition().y, 32, 32)))
				{
					m_powerManager.activate(m_powerManager.m_powers[i], it->second);
					sf::Packet packet;
					packet << 0 << cn::ActivatePower;
					m_socket.send(packet, it->second.getAddress(), it->second.getPort());
					retPacket << cn::PlayerHealth << it->second.getName() << it->second.getHealth();
					m_powerManager.m_powers.erase(m_powerManager.m_powers.begin() + i);
				}
			}
		}
		if (it->second.getHealth() <= 0)
		{
			it->second.m_respawnTime -= m_clock.getElapsedTime().asSeconds();
			if (it->second.m_respawnTime <= 0)
			{
				respawnPlayerPacket(it->second, retPacket);
			}
		}
		m_gameManager.update(it->second);
		retPacket << cn::PlayerMove << it->second.getName() << it->second.getPosition().x << it->second.getPosition().y;
	}
#pragma endregion

	retPacket << cn::Power << m_powerManager.m_powers;

#pragma region Update projectiles
	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); ++it){
		if (it != m_projectiles.end())
		{
			bool erase = false;
			it->move(it->getVelocity());
			erase = m_map.intersectsWall(sf::Rect<float>(it->getPosition().x, it->getPosition().y, 5, 5));
			if (erase)
			{
				//it->setPosition(m_map.getIntersectingWall(sf::Rect<float>(it->getPosition().x, it->getPosition().y, 5, 5)));

				m_eraseProjectileIDs.push_back(it->m_id);
			}else
			{
				m_gameManager.update(*it);
				std::vector<Client> clients = m_gameManager.getClients(*it);

				for (auto iter = clients.begin(); iter != clients.end(); ++iter){
					if (m_gameManager.intersect(*iter, *it))
					{
						if (iter->getName() != it->getName())
						{	
							if (iter->getHealth() > 0)
							{
								m_clientList[iter->getName()].damage(it->m_damage);
							}

							retPacket << cn::PlayerHealth << iter->getName() << m_clientList[iter->getName()].getHealth();

							m_eraseProjectileIDs.push_back(it->m_id);

							if (m_clientList[iter->getName()].getHealth() <= 0) {
								//respawnPlayerPacket(m_clientList[iter->getName()], retPacket);
								m_clientList[iter->getName()].m_score.m_deaths++;
								m_clientList[it->getName()].m_score.m_kills++;
								m_clientList[it->getName()].m_score.m_points++;
							}
						}
					}
				}
			}
		}
	}
#pragma endregion

#pragma region Erase projectiles
	if (!m_eraseProjectileIDs.empty())
	{
		retPacket << cn::EraseProjectile << m_eraseProjectileIDs.size();
		for (auto it = m_eraseProjectileIDs.begin(); it != m_eraseProjectileIDs.end(); ++it){

			std::vector<Projectile>::iterator iter = findID(*it);
			if (iter != m_projectiles.end())
			{
				retPacket << *it << (m_map.intersectsWall(sf::Rect<float>(iter->getPosition().x, iter->getPosition().y, 5, 5)) ? m_map.getIntersectingWall(sf::Rect<float>(iter->getPosition().x, iter->getPosition().y, 5, 5)):iter->getPosition());
				m_gameManager.erase(*iter);
				m_projectiles.erase(iter);
			}
		}
		m_eraseProjectileIDs.clear();
	}
#pragma endregion

#pragma region Send projectiles
	if (!m_projectiles.empty())
	{
		retPacket << cn::Projectile << m_projectiles;
	}
#pragma endregion

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
		if (!m_clientList.empty())
		{
			std::cout << "";
		}

		m_clientList[data] = Client(p_address, p_port);
		m_clientList[data].setName(data);
		sf::Vector2<int> spawn = m_gameManager.selectSpawn(m_map.m_spawnPositions);
		m_clientList[data].setPosition(sf::Vector2<float>(spawn.x*64 + 25, spawn.y*64 + 25));
		m_clientList[data].hasRespondedToPing = true;

		sf::TcpListener listener;
		listener.listen(2828);

		sf::TcpSocket client;
		
		while (listener.accept(client) != sf::Socket::Status::Done)
		{

		}
		

		retPacket << m_elapsed.getElapsedTime().asMilliseconds() << (int)cn::PlayerConnected << data << m_clientList[data].getPosition().x << m_clientList[data].getPosition().y << m_map << m_projectiles << m_powerManager.m_powers;
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

	if (m_projectiles.empty())
	{
		m_projectileID = 0;
	}

	int length;
	std::vector<Projectile> projectiles;
	p_packet >> projectiles;
	if (m_clientList[projectiles.begin()->getName()].getHealth() > 0) {
		sf::Packet retPacket;
		retPacket << 0 << cn::Projectile;
		for (auto it = projectiles.begin(); it != projectiles.end();) {
			if (m_clientList[projectiles.begin()->getName()].shoot()) {
				if (it->m_id == -1) {
					it->m_id = m_projectileID;
					it->m_damage = m_clientList[projectiles.begin()->getName()].getDamage();
					m_projectileID++;
				}
				it->setVelocity(sf::Vector2<float>(it->getVelocity().x*m_projectileSpeed, it->getVelocity().y*m_projectileSpeed));
				m_projectiles.push_back(*it);
				it++;
			} else {
				it = projectiles.erase(it);
			}
		}

		retPacket << projectiles;
		return retPacket;
	}
	return sf::Packet();
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

void Server::respawnPlayerPacket(Client& client, sf::Packet& p_packet)
{
	client.setHealth(100);
	//client.setPosition(sf::Vector2f(map.m_spawnPositions[(math::random(0, map.m_spawnPositions.size() - 1))].x + 25, map.m_spawnPositions[(math::random(0, map.m_spawnPositions.size() - 1))].y + 25));
	sf::Vector2<int> spawn = m_gameManager.selectSpawn(m_map.m_spawnPositions);
	client.setPosition(sf::Vector2<float>(spawn.x*64 + 25, spawn.y*64 + 25));
	p_packet << cn::PlayerRespawn << client.getName() << client.getPosition();
}

sf::Packet Server::ProjectileIDCleanup(sf::Packet & p_packet){
	p_packet << cn::ProjectileIDCleanUp << m_projectiles.size() - 1;
	m_projectileID = m_projectiles.size() - 1;
	for (int i = m_projectiles.size() - 1; i > 0; i--)
	{
		p_packet << m_projectiles[i].m_id;
		m_projectiles[i].m_id = i;
	}
	return p_packet;
}