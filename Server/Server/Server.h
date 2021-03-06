#pragma once
#include <SFML\Network\UdpSocket.hpp>
#include <SFML\Network\TcpSocket.hpp>
#include <SFML\Network\IpAddress.hpp>
#include <SFML\Network\Packet.hpp>
#include <SFML\Network\TcpListener.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\System\String.hpp>
#include <SFML\System\Clock.hpp>

#include <map>
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <thread>

#include "..\..\Common\Protocol.h"
#include "..\..\Common\Map.h"
#include "..\..\Common\Projectile.h"
#include "..\..\Common\GeneralMath.h"

#include "Match.h"
#include "PowerManager.h"
#include "Client.h"
#include "InputData.h"
#include "GameManager.h"
#include "Quadtree.h"
#include "Erase.h"
#include "ChatServer.h"


class Server{
public:
	Server();
	~Server();

private:
	void run();

	sf::Packet simulateGameState();

	void playerConnected(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & p_port);

	void playerDisconnected(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & p_port);

	void playerInput(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & p_port, const int & p_int);

	sf::Packet projectile(sf::Packet & p_packet, const sf::IpAddress & p_address, const unsigned short & p_port, const int & p_int);

	void pingClients();

	sf::Vector2<float> getSpawnPos();

	bool nameTaken(const std::string & p_name);

	///<summary>Inserts a respawn packet to p_packet and respawns a player on the server side</summary>
	void respawnPlayerPacket(Client& player, sf::Packet& p_packet);

	///<summary>Reset the projectile ID to 0. Set the ID of each projectile, and send the new ID to the clients.</summary>
	sf::Packet ProjectileIDCleanup(sf::Packet & p_packet);

	std::vector<Projectile>::iterator findID(const int & p_id);

	bool isMatchOver() const;
	void startMatch();
	int getHightestScore(); // need to fix this 
	void resetScores();

	ChatServer m_chatServer;

	sf::Clock m_clock, m_elapsed;
	sf::Time m_updateTime;
	sf::Clock pingTimer;

	std::vector<InputData> m_clientInputs;

	unsigned short m_port;

	sf::UdpSocket m_socket;
	
	std::map<std::string, Client> m_clientList;

	std::vector<Projectile> m_projectiles;
	
	std::vector<Erase> m_eraseProjectileIDs;

	int m_projectileID;

	Match m_match;
	sf::Time m_timeBetweenMatches;

	void loadConfig();

	Map m_map;

	Quadtree m_quadtree;

	PowerManager m_powerManager;

	GameManager m_gameManager;

	static const unsigned short m_projectileSpeed = 50;

	//%
	static const unsigned short m_multiplier = 15;

	float m_speedBoost;


	//----Simulate Gamestate----

	///<summary>Update playerpositions.</summary>
	void playerMovement();

	///<summary>Fill p_retPacket with power information that the clients need.</summary>
	void updatePowers(sf::Packet & p_retPacket);

	///<summary>Fill p_retPacket with player information that the clients need.</summary>
	void updatePlayers(sf::Packet & p_retPacket);

	///<summary>Fill p_retPakcet with projectile information that the clients need.</summary>
	void updateProjectiles(sf::Packet & p_retPacket);

	///<summary>Fill p_retPacket with score information that the clients need.</summary>
	void sendScore(sf::Packet & p_retPacket);
};