#include "MultiplayerGame.h"
#include "Protocol.h"

#include <SFML\Network\Packet.hpp>

#include <iostream>

MultiplayerGame::MultiplayerGame() :
	m_running(true),
	m_socket()
{
}

MultiplayerGame::~MultiplayerGame()
{
	sf::Packet packet;
	m_socket.send(packet, server_address, server_port);
	m_socket.unbind();
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
		//render();
		while (lag >= updateTime)
		{
			update(updateTime);
			lag -= updateTime;
		}
	}
}

void MultiplayerGame::initialize(sf::IpAddress p_address, unsigned short p_port)
{
	server_address = p_address;
	server_port = p_port;
	int r = 0;
	if (m_socket.bind(sf::UdpSocket::AnyPort) != sf::Socket::Done) {
		std::cout << "Failed to bind to port" << std::endl;	
		exit(-1);
	}
	sf::Packet packet;

	std::string username;
	std::cout << "Username: ";
	std::cin >> username;
	
	packet << (int)cn::PlayerConnected << sf::String(username);
	m_socket.send(packet, p_address, p_port);

	while (m_socket.receive(packet, p_address, p_port) == sf::Socket::Done)
	{
		int type;
		sf::String name;
		packet >> type;

		if ((cn::PacketType)type == cn::PlayerConnected)
		{
			packet >> name;
			if (name == username)
			{
				std::cout << "Connected to server\n";
				break;
			}
		}
	}

	m_socket.setBlocking(false);
	m_window.create(sf::VideoMode(600, 400), "Combat Net");
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
	sf::Packet packet;
	sf::String msg;
	std::string str_msg;
	std::cin >> str_msg;
	msg = str_msg;
	packet << (int)cn::PlayerMessage << msg;

	m_socket.send(packet, server_address, server_port);
}

void MultiplayerGame::render()
{
	m_window.clear();
	m_window.display();
}