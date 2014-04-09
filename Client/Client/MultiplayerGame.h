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

#include "Player.h"
#include "..\..\Common\Map.h"

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

	// Returns true if the packet has been filled with data
	bool handleInput(sf::Packet& packet);

	void handlePlayerConnect(sf::Packet& packet);
	void handlePlayerDisconnect(sf::Packet& packet);
	void handlePlayerMove(sf::Packet& packet);

	sf::RenderWindow m_window;
	sf::View m_view;

	bool m_running, m_active;
	sf::UdpSocket m_socket;

	sf::IpAddress server_address;
	unsigned short server_port;

	sf::String m_name;

	std::map<sf::String, std::unique_ptr<Player>> m_players;

	Map m_map;

	sf::Font gameFont;
};