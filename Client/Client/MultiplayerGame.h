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
#include "Scoreboard.h"
#include "ParticleLoader.h"

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
	void handleProjectile(sf::Packet & p_packet, const int & p_time);
	void handleEraseProjectile(sf::Packet & p_packet);
	void handlePing();
	void handleMegaPacket(sf::Packet & p_packet, int const& p_time);

	sf::RenderWindow m_window;
	sf::View m_view;

	bool m_running, m_active;
	sf::UdpSocket m_socket;

	sf::IpAddress server_address;
	unsigned short server_port;

	std::string m_name;

	std::map<std::string, std::unique_ptr<Player>> m_players;

	std::vector<Projectile> m_projectiles;
	std::vector<Projectile>::iterator findID(const int & p_id);
	std::vector<int> m_eraseProjectileIDs;

	static const unsigned short m_projectileSpeed = 20;

	//%
	static const unsigned short m_multiplier = 15;

	int m_streak;

	Map m_map;

	sf::Font gameFont;

	int m_elapsedGameTime; // this is a delta time, doesn't total elapsedGameTime
	int m_lastServerUpdateTime;
	sf::Clock timeSinceLastServerUpdate;
	sf::Time serverTimeout;

	sf::Text statusText;

	Scoreboard m_scoreboard;

	ParticleLoader m_particleLoader;
};