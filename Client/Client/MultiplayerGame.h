#pragma once

#include <SFML\System\Time.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>

#include <SFML\Network\IpAddress.hpp>
#include <SFML\Network\UdpSocket.hpp>

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

};