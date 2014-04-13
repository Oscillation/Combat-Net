#include "MultiplayerGame.h"
#include "..\..\Common\Protocol.h"

#include <SFML\Network\Packet.hpp>
#include <SFML\Graphics\Text.hpp>

#include <iostream>

MultiplayerGame::MultiplayerGame() :
	m_running(true),
	m_socket(),
	m_window(),
	serverTimeout(sf::milliseconds(500))
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
	gameFont.loadFromFile("Segan-Light.ttf");

	server_address = p_address;
	server_port = p_port;

	if (m_socket.bind(sf::UdpSocket::AnyPort) != sf::Socket::Done) {
		std::cout << "Failed to bind to port" << std::endl;	
		exit(-1);
	}

	while(!connect()){}
}

bool MultiplayerGame::connect(){
	sf::Packet packet;

	std::string username;
	std::cout << "Username: ";
	std::getline(std::cin, username);
	m_name = username;

	packet << 0 << (int)cn::PlayerConnected << sf::String(username);
	m_socket.send(packet, server_address, server_port);

	// Wait for the PlayerConnected packet from the server
	// When the client get's the PlayerConnected packet with the players name
	// it is connected to the server

	while (m_socket.receive(packet, server_address, server_port) == sf::Socket::Done)
	{
		int type, time;
		sf::String name;
		packet >> time >> type;

		if ((cn::PacketType)type == cn::PlayerConnected)
		{
			packet >> name;
			if (name == m_name)
			{
				std::cout << "Connected to server\n";
				sf::Vector2f position;
				packet >> position.x >> position.y;
				packet >> m_map;
				std::unique_ptr<Player> newPlayer(new Player(name, gameFont, false));
				newPlayer->setPosition(position);
				newPlayer->setTargetPosition(position);
				newPlayer->setTargetTime(100);
				m_players[name] = std::move(newPlayer);
				m_socket.setBlocking(false);
				m_window.create(sf::VideoMode(1280, 720), "Combat Net", sf::Style::Close);
				m_view = sf::View(sf::Vector2f(1280/2, 720/2), sf::Vector2f(1280, 720));
				m_view.setCenter(m_players[m_name].get()->getPosition());
				m_view.setCenter(m_players[name].get()->getPosition());
				std::cout << m_name.toAnsiString() << std::endl;
				return true;
				break;
			}
		}else if ((cn::PacketType)type == cn::NameTaken)
		{
			std::cout << "That name is already taken. Please try again with a different name.\n";
			return false;
		}
	}
}

void MultiplayerGame::handleEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
		{
			sf::Packet packet;
			packet << 0 << cn::PlayerDisconnected << m_name;
			m_socket.send(packet, server_address, server_port);
			m_running = false;
			exit(0);
		}else if (event.type == sf::Event::GainedFocus){
			m_active = true;
		} else if (event.type == sf::Event::LostFocus) {
			m_active = false;
		}
	}
}

void MultiplayerGame::update(sf::Time & p_deltaTime)
{
	m_elapsedGameTime += p_deltaTime.asMilliseconds();
	sf::Packet packet;
	sf::IpAddress address;
	unsigned short port;
	while (m_socket.receive(packet, address, port) == sf::Socket::Done)
	{
		if (address != server_address || port != server_port || packet.endOfPacket())
			break;
		int type, time;
		packet >> time >> type;

		if ((cn::PacketType)type == cn::PlayerConnected) 
		{
			handlePlayerConnect(packet);
		}
		else if ((cn::PacketType)type == cn::PlayerDisconnected) 
		{
			handlePlayerDisconnect(packet);
		}else if ((cn::PacketType)type == cn::Ping)
		{
			handlePing();
		}else if ((cn::PacketType)type == cn::MegaPacket)
		{
			handleMegaPacket(packet, time);
		}
		timeSinceLastServerUpdate.restart();
	}

	for (auto it = m_players.begin(); it != m_players.end(); ++it) {
		it->second->update(p_deltaTime, m_elapsedGameTime);
	}

	m_view.setCenter(m_players[m_name]->getPosition());

	if (m_active)
	{
		if (handleInput(packet, p_deltaTime.asMilliseconds()))
		{
			m_socket.send(packet, server_address, server_port);
		}
	}

	// Handle server crash/random disconnect
	if (timeSinceLastServerUpdate.getElapsedTime() > serverTimeout)
	{
		std::cout << "Lost connection to server, exiting" << std::endl;
		sf::sleep(sf::seconds(3));
		m_running = false;
	}
}

void MultiplayerGame::render()
{
	m_window.clear(sf::Color::Black);
	m_window.setView(m_view);

	for (int x = 0, y = 0; x < m_map.m_tiles.size(); x++)
	{
		for (y = 0; y < m_map.m_tiles[x].size(); y++)
		{
			sf::RectangleShape tile = sf::RectangleShape(sf::Vector2f(64, 64));
			tile.setPosition(sf::Vector2f(x*64, y*64));
			switch (m_map.m_tiles[x][y].m_type)
			{
			case Floor:
				tile.setFillColor(sf::Color::Green);
				break;
			case Wall:
				tile.setFillColor(sf::Color::Magenta);
				break;
			default:
				break;
			}
			m_window.draw(tile);
		}
	}

	for (auto i = m_players.begin(); i != m_players.end(); ++i) {
		m_window.draw(*(i->second));
	}

	for (auto it = m_projectileList.begin(); it != m_projectileList.end(); ++it) {
		for (auto iter = it->second.begin(); iter != it->second.end(); ++iter) {
			m_window.draw(*iter->get());
		}
	}

	m_window.display();
}

bool MultiplayerGame::handleInput(sf::Packet& packet, const int & p_deltaTime)
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		inputs.push_back(cn::ShootUp);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		inputs.push_back(cn::ShootDown);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		inputs.push_back(cn::ShootLeft);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		inputs.push_back(cn::ShootRight);
	}

	if (!inputs.empty())
	{
		packet << 0 << cn::PlayerInput << m_name << inputs.size();
		for(auto it = inputs.begin(); it != inputs.end(); ++it){
			packet << (int)*it << p_deltaTime;
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
	std::cout << "Adding: " << name.toAnsiString() << " to list of players.\n";

	std::unique_ptr<Player> newPlayer(new Player(name, gameFont, true));
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
	auto found = m_players.find(name);
	if (found != m_players.end()) {

		m_players[name].get()->setPosition(pos);
		if (name == m_name)
		{
			m_view.setCenter(m_players[name].get()->getPosition());
		}
	}
}

void MultiplayerGame::handlePing()
{
	sf::Packet pingPacket;
	pingPacket << 0 << cn::Ping << m_name;
	m_socket.send(pingPacket, server_address, server_port);
}


void MultiplayerGame::handleMegaPacket(sf::Packet & p_packet, int const& p_time){
	int count = 0;
	p_packet >> count;
	m_elapsedGameTime = 0;
	for (int i = 0; i < count; i++)
	{
		if (!p_packet.endOfPacket())
		{
			int type;
			p_packet >> type;

			if ((cn::PacketType)type == cn::PlayerMove){
				sf::String name;
				p_packet >> name;
				sf::Vector2<float> pos;
				p_packet >> pos.x >> pos.y;
				m_players[name]->setTargetPosition(pos);
				m_players[name]->setTargetTime(p_time);
			}
		}
	}
}