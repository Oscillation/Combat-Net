#include "MultiplayerGame.h"
#include "..\..\Common\Protocol.h"

#include <SFML\Network\Packet.hpp>
#include <SFML\Graphics\Text.hpp>

#include <iostream>

MultiplayerGame::MultiplayerGame(StateStack& stateStack, Context& context, States::ID id) :
	serverTimeout(sf::milliseconds(500)),
	m_scoreboard(m_players),
	m_lastServerUpdateTime(0),
	State(stateStack, context, id),
	m_socket(*context.socket),
	m_connected(false),
	m_active(true),
	gameFont(*context.font)
{
	initialize();

	m_scoreboard.setPosition(1280/2, 300);
	getContext()->scores = &m_players;
}

MultiplayerGame::~MultiplayerGame()
{

}

void MultiplayerGame::initialize()
{
	if (m_socket.bind(sf::UdpSocket::AnyPort) != sf::Socket::Done) {
		std::cout << "Failed to bind to port" << std::endl;
		system("pause");
		exit(-1);
	}

	m_scoreboard.setPosition(1280/2, 300);
	m_scoreboard.setFont(gameFont);

	statusText.setFont(gameFont);
	statusText.setColor(sf::Color::Red);
	statusText.setPosition(30, 30);
	statusText.setStyle(sf::Text::Bold);

	m_respawnTimer.setFont(gameFont);
	m_respawnTimer.setColor(sf::Color::Red);
	m_respawnTimer.setPosition(m_view.getSize().x/2, m_view.getSize().y/2);
	m_respawnTimer.setStyle(sf::Text::Bold);

	server_address = *getContext()->address;
	server_port = getContext()->port;

	m_particleLoader = ParticleLoader("Particles/");
	m_particleEmitter = ParticleEmitter(&m_particleLoader);

	m_listener.setPosition(sf::Vector3<float>(0.5f, 0.5f, 0.f));
}

bool MultiplayerGame::connect(){
	sf::Packet packet;

	sf::TcpSocket tcpSocket;

	m_name = getContext()->username;

	packet << 0 << (int)cn::PlayerConnected << std::string(m_name);

	m_socket.setBlocking(true);
	m_socket.send(packet, server_address, server_port);

	if (tcpSocket.connect(server_address, server_port, sf::seconds(5)) == sf::Socket::Status::Done){
		// Wait for the PlayerConnected packet from the server
		// When the client get's the PlayerConnected packet with the players name
		// it is connected to the server
		if (m_socket.receive(packet, server_address, server_port) == sf::Socket::Done)
		{
			int type, time;
			std::string name;
			packet >> time >> type;

			if ((cn::PacketType)type == cn::PlayerConnected)
			{
				packet >> name;
				std::cout << "Connected to server.\n";
				sf::Vector2f position;
				packet >> position.x >> position.y;
				packet >> m_map;
				packet >> m_projectiles;
				packet >> m_powers;
				std::unique_ptr<Player> newPlayer(new Player(name, gameFont, false));
				newPlayer->setPosition(position);
				newPlayer->setTargetPosition(position);
				newPlayer->setTargetTime(100);
				m_players[name] = std::move(newPlayer);
				m_socket.setBlocking(false);
				m_view = sf::View(sf::Vector2f(1280/2, 720/2), sf::Vector2f(1280, 720));
				m_view.setCenter(m_players[m_name].get()->getPosition());
				std::cout << m_name << std::endl;
				m_connected = true;
				return true;
			}else if ((cn::PacketType)type == cn::NameTaken)
			{
				std::cout << "That name is already taken. Please try again with a different name.\n";
				return false;
			}
		}
	}

	std::cout << "Couldn't connect to server.\n";
	return false;
}

bool MultiplayerGame::handleEvents(const sf::Event& event)
{
	if (event.type == sf::Event::GainedFocus){
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			m_active = true;
		}
	} else if (event.type == sf::Event::LostFocus) {
		m_active = false;
	}
	return false;
}

bool MultiplayerGame::update(sf::Time & p_deltaTime)
{
	if (!m_connected)
	{
		if (!connect()) {
			m_socket.unbind();
			requestStackPop();
		}
	}
	else 
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
			if ((cn::PacketType)type == cn::ActivatePower)
			{
				m_audioPlayer.playSound("power", sf::Vector3<float>((m_players[m_name]->getPosition().x - (m_view.getCenter().x - (m_view.getSize().x/2)))/m_view.getSize().x, (m_players[m_name]->getPosition().y - (m_view.getCenter().y - (m_view.getSize().y/2)))/m_view.getSize().y, 0.f));
				/*sf::Sound* sound = new sf::Sound(m_audioPlayer.m_sounds["power"]);
				sound->setPosition(sf::Vector3<float>((m_players[m_name]->getPosition().x - (m_view.getCenter().x - (m_view.getSize().x/2)))/m_view.getSize().x, (m_players[m_name]->getPosition().y - (m_view.getCenter().y - (m_view.getSize().y/2)))/m_view.getSize().y, 0.f));
				sound->play();*/

			}

			if ((cn::PacketType)type == cn::MatchEnd)
			{
				for (auto it = m_players.begin(); it != m_players.end(); ++it){
					it->second->setHealth(0);
				}

				setRespawnTimer(3.0f);

				requestStackPush(States::PreMatch);
			}
			timeSinceLastServerUpdate.restart();
			statusText.setString("");
		}

		for (auto it = m_players.begin(); it != m_players.end(); ++it) {
			//m_quadtree.m_root.insert(*it->second);
			it->second->update(p_deltaTime, m_elapsedGameTime);
		}

		for (auto it = m_projectiles.begin(); it != m_projectiles.end(); ++it) {
			if (it->m_updated)
			{
				
				it->update(p_deltaTime, m_elapsedGameTime);
			}else
			{
				m_eraseProjectileIDs.push_back(it->m_id);
			}
		}

		for (int i = 0; i < m_eraseProjectileIDs.size(); i++)
		{
			std::vector<Projectile>::iterator it = findID(m_eraseProjectileIDs[i]);
			if (it != m_projectiles.end())
			{
				m_projectiles.erase(it);
			}
		}

		m_eraseProjectileIDs.clear();

		m_particleEmitter.update(p_deltaTime);

		m_view.setCenter(m_players[m_name]->getPosition());
		updateViewShake(p_deltaTime);
		m_view.move(m_viewVelocity);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			m_players[m_name].get()->setPosition(130, 130);
		}

		m_particleEmitter.update(p_deltaTime);

		updateViewShake(p_deltaTime);

		m_view.setCenter(m_players[m_name]->getPosition());

		m_view.move(m_viewVelocity);

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
		else {
			m_scoreboard.deactivate();
		}

		// Handle server crash/random disconnect
		if (timeSinceLastServerUpdate.getElapsedTime() > serverTimeout)
		{
			statusText.setString("Lost connection to server...");
		}

		m_scoreboard.updateStats();	

		if (m_respawnTime > 0)
		{
			m_displayRespawnTimer = true;
			m_respawnTime -= p_deltaTime.asSeconds();
			m_respawnTimer.setString(std::to_string(m_respawnTime));
			m_respawnTimer.setPosition(m_view.getSize().x/2 - m_respawnTimer.getLocalBounds().width/2, m_view.getSize().y/2);
		}else
		{
			m_displayRespawnTimer = false;
		}

		/*m_listener.setPosition(sf::Vector3<float>(m_players[m_name]->getPosition().x/m_view.getSize().x, m_players[m_name]->getPosition().y/m_view.getSize().y, 0));
		m_listener.setDirection(0, 0, 0);*/

		m_audioPlayer.update();
	}

	return false;
}

bool MultiplayerGame::draw()
{
	sf::RenderWindow* window = getContext()->window;
	window->setView(m_view);

	for (unsigned int x = ((m_view.getCenter().x - m_view.getSize().x/2)/64 - 1 >= 0  ? ((m_view.getCenter().x - m_view.getSize().x/2)/64 - 1):0),
		y = ((m_view.getCenter().y - m_view.getSize().y/2)/64 - 1 >= 0  ? ((m_view.getCenter().y - m_view.getSize().y/2)/64 - 1):0);
		x < ((((m_view.getCenter().x + m_view.getSize().x/2)/64 + 1) < m_map.m_tiles.size() ? ((m_view.getCenter().x + m_view.getSize().x/2)/64 + 1):m_map.m_tiles.size())); x++) {
			for (y = ((m_view.getCenter().y - m_view.getSize().y/2)/64 - 1 >= 0  ? ((m_view.getCenter().y - m_view.getSize().y/2)/64 - 1):0);
				y < ((((m_view.getCenter().y + m_view.getSize().y/2)/64 + 1) < m_map.m_tiles.begin()->size() ? ((m_view.getCenter().y + m_view.getSize().y/2)/64 + 1):m_map.m_tiles.begin()->size())); y++) {
					sf::RectangleShape tile = sf::RectangleShape(sf::Vector2f(64, 64));
					tile.setPosition(sf::Vector2f(x*64.f, y*64.f));

					switch (m_map.m_tiles[x][y].m_type)
					{
					case TileType::Floor:
						tile.setFillColor(sf::Color::Black);
						break;
					case TileType::Wall:
						tile.setFillColor(sf::Color::Red);
						break;
					case TileType::Power:
						tile.setFillColor(sf::Color::Black);
						break;
					default:
						tile.setFillColor(sf::Color::Black);
						break;
					}
					window->draw(tile);
			}
	}

	

	for (auto it = m_powers.begin(); it != m_powers.end(); ++it) {
		window->draw(*it);
	}

	for (auto i = m_players.begin(); i != m_players.end(); ++i) {
		if (!i->second.get()->isDead())
		{
			window->draw(*(i->second));
		}
	}

	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); ++it) {
		window->draw(*it);
	}

	window->draw(m_particleEmitter);

	window->setView(window->getDefaultView());
	window->draw(m_scoreboard);
	window->draw(statusText);

	if (m_displayRespawnTimer)
	{
		if (m_respawnTime > 0 && m_players[m_name]->getHealth() == 0)
		{
			window->draw(m_respawnTimer);
		}
	}

	return false;
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
				projectile.setVelocity(sf::Vector2<float>(projectile.getVelocity().x, projectile.getVelocity().y - 1));
			}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				projectile.setVelocity(sf::Vector2<float>(projectile.getVelocity().x, projectile.getVelocity().y + 1));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				projectile.setVelocity(sf::Vector2<float>(projectile.getVelocity().x - 1, projectile.getVelocity().y));
			}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				projectile.setVelocity(sf::Vector2<float>(projectile.getVelocity().x + 1, projectile.getVelocity().y));
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
	if (name != m_name)
	{
		m_players.erase(name);
	}
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
	for (auto it = m_projectiles.begin(); it != m_projectiles.end(); ++it)
	{
		it->m_updated = false;
	}
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
				iter->m_updated = true;
			}else
			{
				Projectile projectile = Projectile(id, 0);
				projectile.setPosition(pos);
				projectile.setTargetPosition(pos);
				projectile.setVelocity(vel);
				projectile.setName(name);
				m_projectiles.push_back(projectile);
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
		sf::Vector2<float> position;
		p_packet >> id >> position;
		std::vector<Projectile>::iterator it = findID(id);
		if (it != m_projectiles.end())
		{
			if (it->getVelocity().x == 0)
			{
				position.x = it->getPosition().x;

				if (it->getVelocity().y < 0)
				{
					position.y += 64;
				}
			}else if (it->getVelocity().y == 0)
			{
				position.y = it->getPosition().y;

				if (it->getVelocity().x < 0)
				{
					position.x += 64;
				}
			}else
			{
				position.x = it->getPosition().x + it->getVelocity().x;
				position.y = it->getPosition().y + it->getVelocity().y;
			}
			sf::Vector2<float> velocity = sf::Vector2<float>(((it->getVelocity().x)/(std::sqrt(std::pow(it->getVelocity().x, 2)) + (std::sqrt(std::pow(it->getVelocity().y, 2)))))*-1,
				((it->getVelocity().y)/(std::sqrt(std::pow(it->getVelocity().x, 2)) + (std::sqrt(std::pow(it->getVelocity().y, 2)))))*-1);
			m_particleEmitter.Emit("test", position, velocity);
			m_particleEmitter.Emit("projectile", position + velocity);

			m_audioPlayer.playSound("projectile_hit_wall", sf::Vector3<float>((position.x - (m_view.getCenter().x - (m_view.getSize().x/2)))/m_view.getSize().x, (position.y - (m_view.getCenter().y - (m_view.getSize().y/2)))/m_view.getSize().y, 0.f));

			/*sf::Sound* sound = new sf::Sound(m_audioPlayer.m_sounds["projectile_hit_wall"]);
			sound->setPosition(sf::Vector3<float>((position.x - (m_view.getCenter().x - (m_view.getSize().x/2)))/m_view.getSize().x, (position.y - (m_view.getCenter().y - (m_view.getSize().y/2)))/m_view.getSize().y, 0.f));
			sound->play();*/
			m_projectiles.erase(it);
			//shakeView(sf::seconds(0.05f));
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
			if (name == m_name)
			{
				if (m_players[name]->getHealth() <= 0)
				{
					setRespawnTimer(3.f);
				}
			}
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
		} else if ((cn::PacketType)type == cn::Power)
		{
			m_powers.clear();
			p_packet >> m_powers;
		}
	}
}

void MultiplayerGame::shakeView(const sf::Time & p_time){
	m_viewShakeTime = p_time.asSeconds();
}

void MultiplayerGame::updateViewShake(const sf::Time & p_deltaTime){
	if (m_viewShakeTime > 0)
	{
		m_viewVelocity.x = math::random(-2.5f, 2.5f);
		m_viewVelocity.y = math::random(-2.5f, 2.5f);
		m_viewShakeTime -= p_deltaTime.asSeconds();
	}else
	{
		m_viewVelocity = sf::Vector2<float>();
	}
}

void MultiplayerGame::setRespawnTimer(const float & p_time){
	m_displayRespawnTimer = true;
	m_respawnTime = p_time;
	m_respawnTimer.setString(std::to_string(m_respawnTime));
}