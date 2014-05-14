#pragma once

#include "StateStack.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Network\UdpSocket.hpp>
#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\Font.hpp>
#include <SFML\Network\IpAddress.hpp>

class Application 
{
public:
	Application();
	~Application();

	void run();

private:

	void initialize();
	void update(sf::Time dt);
	void draw();
	void handleEvents();


	sf::RenderWindow m_window;
	sf::UdpSocket m_socket;
	StateStack m_stateStack;

	bool m_running;
	bool m_active;

	sf::Font m_font;
};