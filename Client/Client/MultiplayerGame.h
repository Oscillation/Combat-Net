#pragma once

#include <SFML\System\Time.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>
#include <SFML\System\Sleep.hpp>

#include <SFML\Network\IpAddress.hpp>
#include <SFML\Network\UdpSocket.hpp>

#include "Player.h"

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

	sf::RenderWindow m_window;
	bool m_running;
	sf::UdpSocket m_socket;

	sf::IpAddress server_address;
	unsigned short server_port;

	sf::String m_name;

	std::map<sf::String, std::unique_ptr<Player>> m_players;
};