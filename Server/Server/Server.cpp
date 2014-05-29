#include "Server.h"

Server::Server() : m_projectileID(0), m_speedBoost(5){

	loadConfig();
	while (m_socket.bind(m_port) != sf::Socket::Done)
	{
		std::cout << "Retry with port: ";
		std::cin >> m_port;
	}
	system("cls");

	std::cout << "Server started.\nNow accepting connections to: " << sf::IpAddress::getPublicAddress() << ":" << m_port << ".\n\n\n\n";

	m_map = Map("Maps/" + m_match.maps[math::random(0, m_match.maps.size() -1)] + ".map");
	m_gameManager = GameManager(&m_clientList, &m_projectiles, m_map.m_tiles.size(), m_map.m_tiles.begin()->size());
	m_powerManager = PowerManager(m_map.getPowerTiles());
	m_quadtree = Quadtree(m_map);

	pingTimer.restart();
	m_clock.restart();
	m_elapsed.restart();
	m_updateTime = sf::milliseconds(50);
	m_timeBetweenMatches = sf::seconds(5);

	if (m_match.type == cn::MatchType::TeamDeathmatch)
	{
		for (int i = 0; i < m_match.teamAmount; ++i)
		{
			m_match.m_teams[i] = 0;
		}
	}

	m_match.active = false;

	run();
}

Server::~Server(){

}

void Server::run(){
	bool run = true;
	m_socket.setBlocking(false);

	startMatch();

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
			if (m_match.active)	
			{
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

		if (m_match.active)
		{
			if (m_clock.getElapsedTime() > m_updateTime)
			{
				retPacket = simulateGameState();
				for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
					m_socket.send(retPacket, it->second.getAddress(), it->second.getPort());
				}
				m_clock.restart();
			}
			if (getHightestScore() >= m_match.pointsToWin)
			{
				m_match.active = false;
				std::cout << "Some dude won :)\n";

				sf::Packet matchDone;

				/*
				*Load new map:
				*/
				{
					m_map.m_tiles.clear();
					m_powerManager.m_powers.clear();
					m_powerManager.m_powerTiles.clear();
					std::string mapPath = "Maps/" + m_match.maps[math::random(0, m_match.maps.size() - 1)] + ".map";
					m_map = Map(mapPath);
					m_powerManager = PowerManager(m_map.getPowerTiles());
				}

				matchDone << 0 << cn::MatchEnd << m_map;
				m_projectiles.clear();
				m_projectileID = 0;
				for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
					m_socket.send(matchDone, it->second.getAddress(), it->second.getPort());
					it->second.setHealth(0);
				}
			}
		}
		else 
		{
			if (m_clock.getElapsedTime() > m_timeBetweenMatches)
			{
				std::cout << "Starting match\n";
				startMatch();
				m_clock.restart();

				sf::Packet matchStart;
				matchStart << 0 << cn::MatchStart;
				for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
					m_socket.send(matchStart, it->second.getAddress(), it->second.getPort());
				}
			}
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

	m_eraseProjectileIDs.clear();
	m_quadtree.clean();

	playerMovement();

	m_clientInputs.clear();

	updatePowers(retPacket);

	updatePlayers(retPacket);

	updateProjectiles(retPacket);

	sendScore(retPacket);

	return retPacket;
}

void Server::playerMovement(){
	for (InputData input : m_clientInputs) {
		Client* client = &m_clientList[input.getPlayer()];
		if (client->getHealth() > 0) {
			float deltaTime = (float)input.getDeltaTime()/1000.f;
			sf::Vector2<float> pos = client->getPosition();

			bool hitWall = false;

			switch (input.getInputType())
			{
			case cn::MoveUp:
				for (int i = ((client->getSpeed()*deltaTime) + (client->m_speedBoost*m_speedBoost*deltaTime)); i > 0; i--)
				{
					if (m_map.intersectsWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y - i), 0))
					{
						client->setPosition(client->getPosition().x, m_map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y - i)).y + 64 + 20);
						hitWall = true;
						break;
					}
				}
				if (!hitWall)
				{
					client->move(0, (-client->getSpeed()*deltaTime) - (client->m_speedBoost*m_speedBoost*deltaTime));
				}
				break;

			case cn::MoveDown:
				for (int i = ((client->getSpeed()*deltaTime) + (client->m_speedBoost*m_speedBoost*deltaTime)); i > 0; i--)
				{
					if (m_map.intersectsWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y + i), 1))
					{
						client->setPosition(client->getPosition().x, m_map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x, client->getPosition().y + i)).y - 20);
						hitWall = true;
						break;
					}
				}
				if (!hitWall)
				{
					client->move(0, client->getSpeed()*deltaTime + (client->m_speedBoost*m_speedBoost*deltaTime));
				}
				break;

			case cn::MoveLeft:
				for (int i = ((client->getSpeed()*deltaTime) + (client->m_speedBoost*m_speedBoost*deltaTime)); i > 0; i--)
				{
					if (m_map.intersectsWall(sf::Vector2<float>(client->getPosition().x - i, client->getPosition().y), 2))
					{
						client->setPosition(m_map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x - i, client->getPosition().y)).x + 64 + 20, client->getPosition().y);
						hitWall = true;
						break;
					}
				}
				if (!hitWall)
				{
					client->move(-client->getSpeed()*deltaTime - (client->m_speedBoost*m_speedBoost*deltaTime), 0);
				}
				break;

			case cn::MoveRight:
				for (int i = ((client->getSpeed()*deltaTime) + (client->m_speedBoost*m_speedBoost*deltaTime)); i > 0; i--)
				{
					if (m_map.intersectsWall(sf::Vector2<float>(client->getPosition().x + i, client->getPosition().y), 3))
					{
						client->setPosition(m_map.getIntersectingWall(sf::Vector2<float>(client->getPosition().x + i, client->getPosition().y)).x - 20, client->getPosition().y);
						hitWall = true;
						break;
					}
				}
				if (!hitWall)
				{
					client->move(client->getSpeed()*deltaTime + (client->m_speedBoost*m_speedBoost*deltaTime), 0);
				}
				break;
			}
		}
	}
}

void Server::updatePowers(sf::Packet & p_retPacket){
	m_powerManager.update(m_clock.getElapsedTime());

	for (auto it = m_powerManager.m_powers.begin(); it != m_powerManager.m_powers.end(); ++it){
		m_quadtree.m_root.insert(*it);
	}

	p_retPacket << cn::Power << m_powerManager.m_powers;
}

void Server::updatePlayers(sf::Packet & p_retPacket){
	for (auto it = m_clientList.begin(); it != m_clientList.end(); ++it){
		m_quadtree.m_root.insert(it->second);
		std::vector<Object> objects;
		m_quadtree.m_root.getObjects(objects, it->second);

		for (auto iter = objects.begin(); iter != objects.end(); ++iter){
			switch (iter->getObjectType())
			{
			case ObjectType::Player:
				break;
			case ObjectType::Projectile:
				break;
			case ObjectType::Power:
				if (math::circleIntersectsRect(sf::Vector2<float>(it->second.getPosition().x - 20.f, it->second.getPosition().y - 20.f), 20.f, iter->getBounds()))
				{
					std::vector<Power>::iterator power = m_powerManager.findId(iter->getId());
					if (power != m_powerManager.m_powers.end())
					{
						bool sendHealth = false;
						if (power->m_powerType == PowerType::Health)
						{
							sendHealth = true;
						}

						sf::Packet packet;
						packet << 0 << cn::ActivatePower << power->m_powerType;
						m_socket.send(packet, it->second.getAddress(), it->second.getPort());

						m_powerManager.activate(*power, it->second);
						m_powerManager.erase(*power);
						if (sendHealth)
						{
							p_retPacket << cn::PlayerHealth << it->second.getName() << it->second.getHealth();
						}
					}
				}
				break;
			default:
				break;
			}
		}

		if (it->second.getHealth() <= 0)
		{
			it->second.m_respawnTime -= m_clock.getElapsedTime().asSeconds();
			if (it->second.m_respawnTime <= 0)
			{
				respawnPlayerPacket(it->second, p_retPacket);
			}
		}

		p_retPacket << cn::PlayerMove << it->second.getName() << it->second.getPosition().x << it->second.getPosition().y;
	}
}

void Server::updateProjectiles(sf::Packet & p_retPacket){
	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); ++it){
		if (it != m_projectiles.end())
		{
			bool erase = false;

			if (it->getVelocity().x != 0 && it->getVelocity().y == 0)
			{
				if (it->getVelocity().x > 0)
				{
					for (int x = it->getVelocity().x; x > 0; --x)
					{
						if (m_map.intersectsWall(sf::Rect<float>(it->getPosition().x + x, it->getPosition().y, 5, 5)))
						{
							erase = true;
							it->setPosition(sf::Vector2<float>(it->getPosition().x + x, it->getPosition().y));
							break;
						}
					}
				}else if (it->getVelocity().x < 0)
				{
					for (int x = it->getVelocity().x; x < 0; ++x)
					{
						if (m_map.intersectsWall(sf::Rect<float>(it->getPosition().x + x, it->getPosition().y, 5, 5)))
						{
							erase = true;
							it->setPosition(sf::Vector2<float>(it->getPosition().x + x, it->getPosition().y));
							break;
						}
					}
				}
			}else if (it->getVelocity().x == 0 && it->getVelocity().y != 0)
			{
				if (it->getVelocity().y > 0)
				{
					for (int y = it->getVelocity().y; y > 0; --y)
					{
						if (m_map.intersectsWall(sf::Rect<float>(it->getPosition().x, it->getPosition().y + y, 5, 5)))
						{
							erase = true;
							it->setPosition(sf::Vector2<float>(it->getPosition().x, it->getPosition().y + y));
							break;
						}
					}
				}else if (it->getVelocity().y < 0)
				{
					for (int y = it->getVelocity().y; y < 0; ++y)
					{
						if (m_map.intersectsWall(sf::Rect<float>(it->getPosition().x, it->getPosition().y + y, 5, 5)))
						{
							erase = true;
							it->setPosition(sf::Vector2<float>(it->getPosition().x, it->getPosition().y + y));
							break;
						}
					}
				}
			}


			if (erase)
			{
				//it->setPosition(m_map.getIntersectingWall(sf::Rect<float>(it->getPosition().x, it->getPosition().y, 5, 5)));
				//it->setVelocity(sf::Vector2<float>());
				m_eraseProjectileIDs.push_back(it->m_id);
			}else
			{
				/*for (int i = std::sqrt(std::pow(it->getVelocity().x, 2) + std::pow(it->getVelocity().y, 2)); i > 0; --i)
				{
				if (m_map.intersectsWall(it->getPosition(), it->getPosition() + sf::Vector2<float>(it->getVelocity().x/i, it->getVelocity().y/i)))
				{
				erase = true;
				it->setPosition(it->getPosition() + sf::Vector2<float>(it->getVelocity().x/i, it->getVelocity().y/i));
				it->setVelocity(sf::Vector2<float>());
				break;
				}
				}*/
				if (it->getVelocity().x != 0 && it->getVelocity().y != 0)
				{
					erase = m_map.intersectsWall(it->getPosition(), it->getPosition() + it->getVelocity());
				}

				if (erase)
				{
					it->setVelocity(sf::Vector2<float>());
					m_eraseProjectileIDs.push_back(it->m_id);
				}else
				{
					it->move(it->getVelocity());

					m_quadtree.m_root.insert(*it);

					std::vector<Object> objects;
					m_quadtree.m_root.getObjects(objects, *it);

					for (auto iter = objects.begin(); iter != objects.end(); ++iter){
						switch (iter->getObjectType())
						{
						case ObjectType::Player:
							if (m_clientList[iter->getName()].getTeam() != m_clientList[it->getName()].getTeam())
							{
								if (math::LineIntersectsCircle(it->getPosition(), it->getPosition() + sf::Vector2<float>(it->getVelocity().x/2, it->getVelocity().y/2), sf::Vector2<float>(iter->getBounds().left, iter->getBounds().top), 20.f) ||
									math::LineIntersectsCircle(it->getPosition(), it->getPosition() + sf::Vector2<float>(it->getVelocity().x, it->getVelocity().y), sf::Vector2<float>(iter->getBounds().left, iter->getBounds().top), 20.f) ||
									math::circleIntersectsRect(sf::Vector2<float>(iter->getBounds().left, iter->getBounds().top), 20.f, sf::Rect<float>(it->getPosition().x, it->getPosition().y, 5, 5)))//if (math::circleIntersectsRect(sf::Vector2<float>(m_clientList[iter->getName()].getPosition().x - 20.f, m_clientList[iter->getName()].getPosition().y - 20.f), 20.f, sf::Rect<float>(it->getPosition().x, it->getPosition().y, 5, 5)))
								{
									if (iter->getName() != it->getName() && m_clientList[iter->getName()].getHealth() > 0)
									{
										if (m_clientList[iter->getName()].getHealth() > 0)
										{
											m_clientList[iter->getName()].damage(it->m_damage);
										}

										p_retPacket << cn::PlayerHealth << iter->getName() << m_clientList[iter->getName()].getHealth();

										m_eraseProjectileIDs.push_back(it->m_id);

										if (m_clientList[iter->getName()].getHealth() <= 0) {
											m_clientList[iter->getName()].m_score.m_deaths++;
											m_clientList[it->getName()].m_score.m_kills++;
											m_clientList[it->getName()].m_score.m_points++;

											p_retPacket << cn::PlayerKilled << it->getName() << iter->getName();
										}
									}
								}
							}
							break;
						case ObjectType::Projectile:
							break;
						case ObjectType::Power:
							break;
						default:
							break;
						}
					}
				}

				//m_gameManager.update(*it);
				//std::vector<Client> clients = m_gameManager.getClients(*it);

				//for (auto iter = clients.begin(); iter != clients.end(); ++iter){
				//	if (m_gameManager.intersect(*iter, *it))
				//	{
				//		if (iter->getName() != it->getName())
				//		{	
				//			if (iter->getHealth() > 0)
				//			{
				//				m_clientList[iter->getName()].damage(it->m_damage);
				//			}

				//			retPacket << cn::PlayerHealth << iter->getName() << m_clientList[iter->getName()].getHealth();

				//			m_eraseProjectileIDs.push_back(it->m_id);

				//			if (m_clientList[iter->getName()].getHealth() <= 0) {
				//				//respawnPlayerPacket(m_clientList[iter->getName()], retPacket);
				//				m_clientList[iter->getName()].m_score.m_deaths++;
				//				m_clientList[it->getName()].m_score.m_kills++;
				//				m_clientList[it->getName()].m_score.m_points++;
				//			}
				//		}
				//	}
				//}
			}
		}
	}
	if (!m_eraseProjectileIDs.empty())
	{
		p_retPacket << cn::EraseProjectile << m_eraseProjectileIDs.size();
		for (auto it = m_eraseProjectileIDs.begin(); it != m_eraseProjectileIDs.end(); ++it){

			std::vector<Projectile>::iterator iter = findID(*it);
			if (iter != m_projectiles.end())
			{
				p_retPacket << *it << (m_map.intersectsWall(sf::Rect<float>(iter->getPosition().x, iter->getPosition().y, 5, 5)) ? m_map.getIntersectingWall(sf::Rect<float>(iter->getPosition().x, iter->getPosition().y, 5, 5)):iter->getPosition());

				//m_gameManager.erase(*iter);
				m_projectiles.erase(iter);
			}
		}
		m_eraseProjectileIDs.clear();
	}

	if (!m_projectiles.empty())
	{
		p_retPacket << cn::Projectile << m_projectiles;
	}
}

void Server::sendScore(sf::Packet & p_retPacket){
	for (auto i = m_clientList.begin(); i != m_clientList.end(); i++)
	{
		p_retPacket << cn::ScoreUpdate << i->second.getName() << i->second.m_score;
	}
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
		sf::Vector2<int> spawn = m_gameManager.selectSpawn(m_map.m_spawnPositions);
		m_clientList[data].setPosition(sf::Vector2<float>(spawn.x*64 + 25, spawn.y*64 + 25));
		m_clientList[data].hasRespondedToPing = true;

		sf::TcpListener listener;
		listener.listen(2828);

		sf::TcpSocket client;
		client.setBlocking(true);

		listener.accept(client);

		int team;
		int low = 0;

		bool broken = false;

		switch (m_match.type)
		{
		case cn::MatchType::FreeForAll:
			for (int i = 0; i < m_match.m_teams.size(); ++i)
			{
				if (m_match.m_teams[i] <= 0)
				{
					team = m_match.m_teams[i];
					m_match.m_teams[i]++;
					broken = true;
					break;
				}
				if (broken)
				{
					break;
				}
			}
			if (!broken)
			{
				team = m_match.m_teams.size();
				m_match.m_teams[m_match.m_teams.size()]++;
			}
			break;
		case cn::MatchType::TeamDeathmatch:
			for (int i = 0; i < m_match.m_teams.size(); i++)
			{
				if (m_match.m_teams[i] < m_match.m_teams[low])
				{
					low = i;
				}
			}
			team = low;
			m_match.m_teams[low]++;
			break;
		default:
			break;
		}

		std::cout << team << "\n";

		retPacket << m_elapsed.getElapsedTime().asMilliseconds() << cn::PlayerConnected << data << m_clientList[data].getPosition().x << m_clientList[data].getPosition().y << m_map << m_projectiles << m_powerManager.m_powers << team;
		m_socket.send(retPacket, p_address, p_port);

		//Send connecting client to already connected clients
		for (auto i = m_clientList.begin(); i != m_clientList.end(); i++)
		{
			if (i->first != data) {
				sf::Packet specialDelivery;
				specialDelivery << m_elapsed.getElapsedTime().asMilliseconds() << cn::PlayerConnected << data << m_clientList[data].getPosition().x << m_clientList[data].getPosition().y << team;
				m_socket.send(specialDelivery, i->second.getAddress(), i->second.getPort());
			}
		}

		//Send clients to connecting client
		for (auto i = m_clientList.begin(); i != m_clientList.end(); i++)
		{
			if (i->first != data) {
				sf::Packet specialDelivery;
				specialDelivery << m_elapsed.getElapsedTime().asMilliseconds() << cn::PlayerConnected << i->first << m_clientList[i->first].getPosition().x << m_clientList[i->first].getPosition().y << m_clientList[i->first].getTeam();
				m_socket.send(specialDelivery, p_address, p_port);
			}
		}

		retPacket.clear();

		m_clientList[data].setTeam(team);

		std::cout << from << data << " has connected.\n";
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
	m_match.m_teams[m_clientList[name].getTeam()]--;
	m_clientList.erase(name);
	sf::Packet packet;
	packet << 0 << cn::PlayerDisconnected << name;
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
					it->setPosition(m_clientList[projectiles.begin()->getName()].getPosition());
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
			m_match.m_teams[m_clientList[it->first].getTeam()]--;
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

bool Server::isMatchOver() const
{
	return m_match.active;
}

void Server::startMatch()
{
	resetScores();
	m_match.active = true;
}

int Server::getHightestScore()
{
	int maxScore = 0;
	std::map<int, int> teamScores;
	for (auto i = m_clientList.begin(); i != m_clientList.end(); ++i)
	{
		teamScores[i->second.getTeam()] += i->second.m_score.m_points;
		maxScore = (teamScores[i->second.getTeam()] > maxScore) ? teamScores[i->second.getTeam()] : maxScore;
	}
	return maxScore;
}

void Server::resetScores()
{
	for (auto i = m_clientList.begin(); i != m_clientList.end(); ++i)
	{
		i->second.m_score.m_points = 0;
		i->second.m_score.m_kills = 0;
		i->second.m_score.m_deaths = 0;
	}
}

void Server::loadConfig()
{
	std::ifstream file;
	file.open("server.cfg");

	if (file.is_open())
	{
		int currentLine = 0;
		std::string line;
		std::istringstream mapNames;
		std::string currentMap;
		while (!file.eof()) {
			std::getline(file, line);
			currentLine++;

			switch (currentLine) {
			case 1:
				m_match.type = (cn::MatchType)std::atoi(line.c_str());
				break;

			case 2:
				m_match.teamAmount = std::atoi(line.c_str());

				//init teams
				for (int i = 0; i < m_match.teamAmount; i++)
				{
					m_match.m_teams[i] = 0;
				}
				break;

			case 3:
				m_match.maxPlayers = std::atoi(line.c_str());
				break;

			case 4:
				m_match.pointsToWin = std::atoi(line.c_str());
				break;

			case 5:
				mapNames.str(line);
				while (std::getline(mapNames, currentMap, ';')){
					m_match.maps.push_back(currentMap);
				}
				break;

			case 6:
				m_port = std::atoi(line.c_str());
				break;

			default:
				std::cout << "Unexpected line" << currentLine << "\n";
				break;
			}
		}
	}
	else
	{
		std::cout << "Failed to open server.cfg\nUsing default configuration.\n";
		m_match.type = cn::FreeForAll;
		m_match.maxPlayers = 10;
		m_match.maps.push_back("map.map");
	}
	file.close();
}