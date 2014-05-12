#include "MultiplayerGame.h"
#include "..\..\Common\Protocol.h"

#include <SFML\Network\Packet.hpp>
#include <SFML\Graphics\Text.hpp>

#include <iostream>

MultiplayerGame::MultiplayerGame() :
	m_running(true),
	m_socket(),
	m_window(),
	serverTimeout(sf::milliseconds(500)),
	m_streak(0),
	m_scoreboard(m_players),
	m_lastServerUpdateTime(0)
{
	m_scoreboard.setPosition(1280/2, 300);
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
	m_scoreboard.setFont(gameFont);

	statusText.setFont(gameFont);
	statusText.setColor(sf::Color::Red);
	statusText.setPosition(30, 30);
	statusText.setStyle(sf::Text::Bold);

	server_address = p_address;
	server_port = p_port;

	m_particleLoader = ParticleLoader("Particles/");
	m_particleEmitter = ParticleEmitter(&m_particleLoader);

	if (m_socket.bind(sf::UdpSocket::AnyPort) != sf::Socket::Done) {
		std::cout << "Failed to bind to port" << std::endl;
		system("pause");
		exit(-1);
	}

	while(!connect()){

	}
}

bool MultiplayerGame::connect(){
	sf::Packet packet;

	std::string username;
	std::cout << "Username: ";
	std::getline(std::cin, username);
	m_name = username;

	packet << 0 << (int)cn::PlayerConnected << std::string(username);
	m_socket.send(packet, server_address, server_port);

	// Wait for the PlayerConnected packet from the server
	// When the client get's the PlayerConnected packet with the players name
	// it is connected to the server

	while (m_socket.receive(packet, server_address, server_port) == sf::Socket::Done)
	{
		int type, time;
		std::string name;
		packet >> time >> type;

		if ((cn::PacketType)type == cn::PlayerConnected)
		{
			packet >> name;
			if (name == m_name)
			{
				std::cout << "Connected to server.\n";
				sf::Vector2f position;
				packet >> position.x >> position.y;
				packet >> m_map;
				packet >> m_projectiles;
				std::unique_ptr<Player> newPlayer(new Player(name, gameFont, false));
				newPlayer->setPosition(position);
				newPlayer->setTargetPosition(position);
				newPlayer->setTargetTime(100);
				m_players[name] = std::move(newPlayer);
				m_socket.setBlocking(false);
				m_window.create(sf::VideoMode(1280, 720), "Combat Net", sf::Style::Close);
				m_view = sf::View(sf::Vector2f(1280/2, 720/2), sf::Vector2f(1280, 720));
				m_view.setCenter(m_players[m_name].get()->getPosition());
				std::cout << m_name << std::endl;
				return true;
			}
		}else if ((cn::PacketType)type == cn::NameTaken)
		{
			std::cout << "That name is already taken. Please try again with a different name.\n";
			return false;
		}
	}
	std::cout << "Couldn't connect to server.\n";
	return false;
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
		if ((cn::PacketType)type == cn::PlayerDisconnected) 
		{
			handlePlayerDisconnect(packet);
		}
		if ((cn::PacketType)type == cn::Ping) 
		{
			handlePing();
		}
		if ((cn::PacketType)type == cn::MegaPacket)
		{
			if (time > m_lastServerUpdateTime)
			{
				handleMegaPacket(packet, time);
				m_lastServerUpdateTime = time;
			}
		}
		timeSinceLastServerUpdate.restart();
	}

	for (auto it = m_players.begin(); it != m_players.end(); ++it) {
		it->second->update(p_deltaTime, m_elapsedGameTime);
	}

	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); ++it) {
		it->update(p_deltaTime, m_elapsedGameTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		m_particleEmitter.Emit("test", m_players[m_name]->getPosition());
	}

	m_particleEmitter.update(p_deltaTime);

	updateViewShake(p_deltaTime);

	m_view.setCenter(m_players[m_name]->getPosition());

	m_view.move(m_viewVelocity);

	if (m_active)
	{
		sf::Packet inputPacket;
		sf::Packet projectilePacket;
		if (handleInput(inputPacket, p_deltaTime.asMilliseconds()))
		{
			m_socket.send(inputPacket, server_address, server_port);
		}
		if (handleProjectileInput(projectilePacket, p_deltaTime.asMilliseconds()))
		{
			m_socket.send(projectilePacket, server_address, server_port);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
			m_scoreboard.activate();
		else 
			m_scoreboard.deactivate();
	}else 
		m_scoreboard.deactivate();

	// Handle server crash/random disconnect
	if (timeSinceLastServerUpdate.getElapsedTime() > serverTimeout)
	{
		statusText.setString("Lost connection to server...");
	}

	m_scoreboard.updateStats();
}

void MultiplayerGame::render()
{
	m_window.clear(sf::Color::Black);
	m_window.setView(m_view);

	for (unsigned int x = ((m_view.getCenter().x - m_view.getSize().x/2)/64 - 1 >= 0  ? ((m_view.getCenter().x - m_view.getSize().x/2)/64 - 1):0),
		y = ((m_view.getCenter().y - m_view.getSize().y/2)/64 - 1 >= 0  ? ((m_view.getCenter().y - m_view.getSize().y/2)/64 - 1):0);
		x < ((((m_view.getCenter().x + m_view.getSize().x/2)/64 + 1) < m_map.m_tiles.size() ? ((m_view.getCenter().x + m_view.getSize().x/2)/64 + 1):m_map.m_tiles.size())); x++) {
			for (y = ((m_view.getCenter().y - m_view.getSize().y/2)/64 - 1 >= 0  ? ((m_view.getCenter().y - m_view.getSize().y/2)/64 - 1):0);
				y < ((((m_view.getCenter().y + m_view.getSize().y/2)/64 + 1) < m_map.m_tiles.begin()->size() ? ((m_view.getCenter().y + m_view.getSize().y/2)/64 + 1):m_map.m_tiles.begin()->size())); y++) {
					sf::RectangleShape tile = sf::RectangleShape(sf::Vector2f(64, 64));
					tile.setPosition(sf::Vector2f(x*64.f, y*64.f));

					switch (m_map.m_tiles[x][y].m_type)
					{
					case Floor:
						tile.setFillColor(sf::Color::Black);
						break;
					case Wall:
						tile.setFillColor(sf::Color::Red);
						break;
					default:
						tile.setFillColor(sf::Color::Black);
						break;
					}
					m_window.draw(tile);
			}
	}

	for (auto i = m_players.begin(); i != m_players.end(); ++i) {
		if (!i->second.get()->isDead())
		{
			m_window.draw(*(i->second));
		}
	}

	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); ++it) {
		m_window.draw(*it);
	}

	m_window.draw(m_particleEmitter);

	m_window.setView(m_window.getDefaultView());
	m_window.draw(m_scoreboard);
	m_window.draw(statusText);

	m_window.display();
}

bool MultiplayerGame::handleInput(sf::Packet& packet, const int & p_deltaTime)
{
	if (!m_players[m_name]->isDead()) {
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
			packet << 0 << cn::PlayerInput << m_name << inputs.size();
			for(auto it = inputs.begin(); it != inputs.end(); ++it){
				packet << (int)*it << p_deltaTime;
			}
			return true;
		}
	}

	return false;
}

bool MultiplayerGame::handleProjectileInput(sf::Packet& packet, const int & p_deltaTime)
{
	if (!m_players[m_name]->isDead())
	{
		std::vector<Projectile> projectiles;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			Projectile projectile = Projectile();
			projectile.setName(m_name);
			projectile.setPosition(m_players[m_name].get()->getPosition());
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				projectile.setVelocity(sf::Vector2<float>(projectile.getVelocity().x, projectile.getVelocity().y - (m_projectileSpeed + (m_projectileSpeed*((m_multiplier/100)*m_streak)))));
			}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				projectile.setVelocity(sf::Vector2<float>(projectile.getVelocity().x, projectile.getVelocity().y + (m_projectileSpeed + (m_projectileSpeed*((m_multiplier/100)*m_streak)))));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				projectile.setVelocity(sf::Vector2<float>(projectile.getVelocity().x - (m_projectileSpeed + (m_projectileSpeed*((m_multiplier/100)*m_streak))), projectile.getVelocity().y));
			}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				projectile.setVelocity(sf::Vector2<float>(projectile.getVelocity().x + (m_projectileSpeed + (m_projectileSpeed*((m_multiplier/100)*m_streak))), projectile.getVelocity().y));
			}

			projectiles.push_back(projectile);
		}

		if (!projectiles.empty())
		{
			packet << 0 << cn::Projectile << projectiles;
			return true;
		}
	}

	return false;
}

void MultiplayerGame::handlePlayerConnect(sf::Packet& packet)
{
	std::string name;
	sf::Vector2f position;
	packet >> name >> position.x >> position.y;
	std::cout << "Adding: " << name << " to list of players.\n";

	std::unique_ptr<Player> newPlayer(new Player(name, gameFont, true));
	newPlayer->setPosition(position);
	m_players[name] = std::move(newPlayer);
}

void MultiplayerGame::handlePlayerDisconnect(sf::Packet& packet)
{
	std::string name;
	packet >> name;
	m_players.erase(name);
}

void MultiplayerGame::handlePlayerMove(sf::Packet& packet)
{
	std::string name;
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

void MultiplayerGame::handleProjectile(sf::Packet& p_packet, const int & p_time)
{
	int length;
	std::string name;
	p_packet >> length;
	if (length > 0)
	{
		p_packet >> name;
		for (int i = 0; i < length; i++)
		{
			int id;
			sf::Vector2<float> pos, vel;
			p_packet >> id >> pos >> vel;
			std::vector<Projectile>::iterator iter = findID(id);
			if (iter != m_projectiles.end())
			{
				iter->setTargetPosition(pos);
				iter->setVelocity(vel);
				iter->setTargetTime(p_time);
			}else
			{
				Projectile projectile = Projectile(id, 0);
				projectile.setPosition(pos);
				projectile.setTargetPosition(pos);
				projectile.setVelocity(vel);
				projectile.setName(name);
				m_projectiles.push_back(projectile);
				m_streak++;
			}
		}
	}
}

void MultiplayerGame::handleEraseProjectile(sf::Packet & p_packet){
	int length;
	p_packet >> length;

	for (int i = 0; i < length; i++)
	{
		int id;
		p_packet >> id;
		std::vector<Projectile>::iterator it = findID(id);
		if (it != m_projectiles.end())
		{
			m_particleEmitter.Emit("test", it->getPosition(), 
				sf::Vector2<float>(((it->getVelocity().x)/(std::sqrt(std::pow(it->getVelocity().x, 2)) + (std::sqrt(std::pow(it->getVelocity().y, 2)))))*-1,
				((it->getVelocity().y)/(std::sqrt(std::pow(it->getVelocity().x, 2)) + (std::sqrt(std::pow(it->getVelocity().y, 2)))))*-1),
				5);
			shakeView(sf::seconds(0.15f));
			m_projectiles.erase(it);
		}
	}
}

std::vector<Projectile>::iterator MultiplayerGame::findID(const int & p_id){
	for (std::vector<Projectile>::iterator it = m_projectiles.begin(); it != m_projectiles.end(); ++it){
		if (it->m_id == p_id)
		{
			return it;
		}
	}
	return m_projectiles.end();
}

void MultiplayerGame::handlePing()
{
	sf::Packet pingPacket;
	pingPacket << 0 << cn::Ping << m_name;
	m_socket.send(pingPacket, server_address, server_port);
}

void MultiplayerGame::handleMegaPacket(sf::Packet & p_packet, int const& p_time){
	m_elapsedGameTime = 0;
	while (!p_packet.endOfPacket())
	{
		int type;
		p_packet >> type;

		if ((cn::PacketType)type == cn::PlayerMove){
			std::string name;
			p_packet >> name;
			sf::Vector2<float> pos;
			p_packet >> pos.x >> pos.y;
			m_players[name]->setTargetPosition(pos);
			m_players[name]->setTargetTime(p_time);
		}else if ((cn::PacketType)type == cn::EraseProjectile)
		{
			handleEraseProjectile(p_packet);
		}else if ((cn::PacketType)type == cn::Projectile)
		{
			handleProjectile(p_packet, p_time);
		}else if ((cn::PacketType)type == cn::PlayerHealth)
		{
			std::string name;
			int health;
			p_packet >> name >> health;
			m_players[name]->setHealth(health);

		}else if ((cn::PacketType)type == cn::ScoreUpdate)
		{
			std::string name;
			p_packet >> name;
			p_packet >> m_players[name]->m_score;
		} else if ((cn::PacketType)type == cn::PlayerRespawn) {
			std::string name;
			sf::Vector2f position;
			p_packet >> name >> position;
			m_players[name]->setPosition(position);
			m_players[name]->setHealth(100);
		}
		/*else if ((cn::PacketType)type == cn::ProjectileIDCleanUp)
		{
			int size;
			p_packet >> size;
			for (int i = 0; i < size; i++)
			{
				int at, to;
				p_packet >> at >> to;
				std::cout << m_projectiles.back().m_id << "\n";
				{
					findID(at)->m_id = to;
				}
			}
		}*/
	}
}

void MultiplayerGame::shakeView(const sf::Time & p_time){
	m_viewShakeTime = p_time.asSeconds();
}

void MultiplayerGame::updateViewShake(const sf::Time & p_deltaTime){
	if (m_viewShakeTime > 0)
	{
		m_viewVelocity.x = math::random(-5, 5);
		m_viewVelocity.y = math::random(-5, 5);
		m_viewShakeTime -= p_deltaTime.asSeconds();
	}else
	{
		m_viewVelocity = sf::Vector2<float>();
	}
}