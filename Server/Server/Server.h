#pragma once
#include <SFML\Network.hpp>
#include <map>
#include <iostream>
#include <memory>

#include "..\..\Common\Protocol.h"
#include "..\..\Common\Map.h"
#include "..\..\Common\Projectile.h"
#include "GeneralMath.h"
#include "Client.h"

#include <SFML\System\Vector2.hpp>
#include <SFML\System\String.hpp>
#include <SFML\System\Clock.hpp>
#include "InputData.h"


class Server{
public:
	Server(const unsigned short & p_port);
	~Server();

private:
	void run();

	sf::Packet simulateGameState();

	void playerConnected(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & p_port);
	void playerDisconnected(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & p_port);
	void playerInput(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & p_port, const int & p_int);

	void pingClients();

	sf::Vector2<float> getSpawnPos();

	bool nameTaken(const std::string & p_name);

private:
	sf::Clock m_clock, m_elapsed;
	sf::Time m_updateTime;
	sf::Clock pingTimer;

	std::vector<InputData> m_clientInputs;

	unsigned short m_port;

	sf::TcpListener m_listener;
	sf::UdpSocket m_socket;
	
	std::map<sf::String, Client> m_clientList;
	std::map<sf::String, std::vector<std::unique_ptr<Projectile>>> m_projectileList;

	Map map;
};