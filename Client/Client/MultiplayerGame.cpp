#include "MultiplayerGame.h"
#include "..\..\Common\Protocol.h"

#include <SFML\Network\Packet.hpp>

#include <iostream>

MultiplayerGame::MultiplayerGame() :
	m_running(true),
	m_socket(),
	m_window()
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
	m_name = username;

	packet << (int)cn::PlayerConnected << sf::String(username);
	m_socket.send(packet, p_address, p_port);

	// Wait for the PlayerConnected packet from the server
	// When the client get's the PlayerConnected packet with the players name
	// it is connected to the server

	while (m_socket.receive(packet, p_address, p_port) == sf::Socket::Done)
	{
		int type;
		sf::String name;
		packet >> type;

		if ((cn::PacketType)type == cn::PlayerConnected)
		{
			packet >> name;
			if (name == m_name)
			{
				std::cout << "Connected to server\n";
				sf::Vector2f position;
				packet >> position.x >> position.y;
				std::unique_ptr<Player> newPlayer(new Player(false));
				newPlayer->setPosition(position);
				m_players[name] = std::move(newPlayer);
				break;
			}
		}
	}

	m_socket.setBlocking(false);
	m_window.create(sf::VideoMode(600, 400), "Combat Net", sf::Style::Close);
}

void MultiplayerGame::handleEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
		{
			sf::Packet packet;
			packet << cn::PlayerDisconnected << m_name;
			m_socket.send(packet, server_address, server_port);
			m_running = false;
			exit(0);
		}
	}
}

void MultiplayerGame::update(sf::Time & p_deltaTime)
{
	sf::Packet packet;
	sf::IpAddress address;
	unsigned short port;
	while (m_socket.receive(packet, address, port) == sf::Socket::Done)
	{
		if (address != server_address || port != server_port || packet.endOfPacket())
			break;

		int type;
		packet >> type;
		if ((cn::PacketType)type == cn::PlayerConnected) 
		{
			handlePlayerConnect(packet);
		}
		else if ((cn::PacketType)type == cn::PlayerMove)
		{
			handlePlayerMove(packet);
		}
		else if ((cn::PacketType)type == cn::PlayerDisconnected) 
		{
			handlePlayerDisconnect(packet);
		}
	}
	
	sf::Packet send_packet;
	bool shouldSend = handleInput(send_packet);

	if (shouldSend)
		m_socket.send(send_packet, server_address, server_port);
}

void MultiplayerGame::render()
{
	m_window.clear(sf::Color::Black);

	for (auto i = m_players.begin(); i != m_players.end(); ++i) {
		m_window.draw(*(i->second));
	}

	m_window.display();
}

bool MultiplayerGame::handleInput(sf::Packet& packet)
{
	std::vector<cn::InputType> inputs;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		inputs.push_back(cn::MoveUp);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		inputs.push_back(cn::MoveDown);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		inputs.push_back(cn::MoveLeft);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		inputs.push_back(cn::MoveRight);
	}

	if (!inputs.empty())
	{
		packet << cn::PlayerInput << m_name << inputs.size();
		for(auto it = inputs.begin(); it != inputs.end(); ++it){
			packet << *it;
		}
		return true;
	}
	return false;
}

void MultiplayerGame::handlePlayerConnect(sf::Packet& packet)
{
	sf::String name;
	sf::Vector2f position;
	packet >> name >> position.x >> position.y;
	std::unique_ptr<Player> newPlayer(new Player(true));
	newPlayer->setPosition(position);
	m_players[name] = std::move(newPlayer);
}


void MultiplayerGame::handlePlayerDisconnect(sf::Packet& packet)
{
	sf::String name;
	packet >> name;
	m_players.erase(name);
}

void MultiplayerGame::handlePlayerMove(sf::Packet& packet)
{
	sf::String name;
	sf::Vector2f pos;
	packet >> name >> pos.x >> pos.y;
	m_players[name].get()->setPosition(pos);
}