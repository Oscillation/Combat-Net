#include "MultiplayerGame.h"
#include "Protocol.h"

#include <SFML\Network\Packet.hpp>

#include <iostream>

MultiplayerGame::MultiplayerGame() : 
	m_window(sf::VideoMode(1280, 720), "Combat Net", sf::Style::Close),
	m_running(true),
	m_socket()
{
}

MultiplayerGame::~MultiplayerGame()
{
	m_window.close();
}

void MultiplayerGame::run(sf::IpAddress p_address, unsigned short p_port)
{
	initialize(p_address, p_port);

	sf::Clock dt;
	sf::Time lag;
	sf::Time updateTime = sf::seconds(1.f/60.f);

	while (m_running)
	{
		lag += dt.getElapsedTime();
		dt.restart();

		handleEvents();
		render();
		while (lag >= updateTime)
		{
			update(updateTime);
			lag -= updateTime;
		}
	}
}

void MultiplayerGame::initialize(sf::IpAddress p_address, unsigned short p_port)
{
	if (m_socket.bind(sf::UdpSocket::AnyPort) != sf::Socket::Done) {
		std::cout << "Failed to bind socket" << std::endl;
		exit(-1);
	}
	sf::Packet packet;
	packet << sf::String("nigga");
	m_socket.send(packet, p_address, p_port);
}

void MultiplayerGame::handleEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
		{
			m_running = false;
		}
	}
}

void MultiplayerGame::update(sf::Time & p_deltaTime)
{

}

void MultiplayerGame::render()
{
	m_window.clear();
	m_window.display();
}