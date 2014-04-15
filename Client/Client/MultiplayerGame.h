#pragma once

#include <SFML\System\Time.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Window\Event.hpp>
#include <SFML\System\Sleep.hpp>

#include <SFML\Network\IpAddress.hpp>
#include <SFML\Network\UdpSocket.hpp>

#include <SFML\Graphics\Font.hpp>

#include <SFML\System\Clock.hpp>

#include "Player.h"
#include "..\..\Common\Map.h"
#include "..\..\Common\Projectile.h"

#include <map>
#include <memory>

class MultiplayerGame
{
public:
	MultiplayerGame();
	~MultiplayerGame();

	void run(sf::IpAddress p_address, unsigned short p_port);

private:
	
	void initialize(sf::IpAddress p_address, unsigned short p_port);

	void update(sf::Time & p_deltaTime);
	void render();

	void handleEvents();

private:
	bool connect();

	// Returns true if the packet has been filled with data
	bool handleInput(sf::Packet & p_packet, const int & p_deltaTime);
	bool handleProjectileInput(sf::Packet & p_packet, const int & p_deltaTime);
		
	void handlePlayerConnect(sf::Packet & p_packet);
	void handlePlayerDisconnect(sf::Packet & p_packet);
	void handlePlayerMove(sf::Packet & p_packet);
	void handleProjectile(sf::Packet & p_packet);
	void handleEraseProjectile(sf::Packet & p_packet);
	void handlePing();
	void handleMegaPacket(sf::Packet & p_packet, int const& p_time);

	sf::RenderWindow m_window;
	sf::View m_view;

	bool m_running, m_active;
	sf::UdpSocket m_socket;

	sf::IpAddress server_address;
	unsigned short server_port;

	sf::String m_name;

	std::map<sf::String, std::unique_ptr<Player>> m_players;

	std::vector<Projectile> m_projectiles;
	std::vector<Projectile>::iterator findID(const int & p_id);
	std::vector<int> m_eraseProjectileIDs;

	Map m_map;

	sf::Font gameFont;

	int m_elapsedGameTime; // this is a delta time, doesn't total elapsedGameTime
	sf::Clock timeSinceLastServerUpdate;
	sf::Time serverTimeout;
};