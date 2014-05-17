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

#include <SFML\System\Clock.hpp>

#include "Player.h"
#include "..\..\Common\Map.h"
#include "..\..\Common\Projectile.h"
#include "Scoreboard.h"
#include "ParticleLoader.h"
#include "ParticleEmitter.h"

#include <map>
#include <memory>

#include "State.h"

class MultiplayerGame : public State
{
public:
	MultiplayerGame(StateStack& stateStack, Context& context, States::ID id);
	virtual ~MultiplayerGame();

	bool update(sf::Time & p_deltaTime);
	bool draw();
	bool handleEvents(const sf::Event& event);

private:
	
	void initialize();
 
	bool connect();

	// Returns true if the packet has been filled with data
	bool handleInput(sf::Packet & p_packet, const int & p_deltaTime);
	bool handleProjectileInput(sf::Packet & p_packet, const int & p_deltaTime);
		
	void handlePlayerConnect(sf::Packet & p_packet);
	void handlePlayerDisconnect(sf::Packet & p_packet);
	void handlePlayerMove(sf::Packet & p_packet);
	void handleProjectile(sf::Packet & p_packet, const int & p_time);
	void handleEraseProjectile(sf::Packet & p_packet);
	void handlePing();
	void handleMegaPacket(sf::Packet & p_packet, int const& p_time);

	sf::View m_view;

	///<summary>Screen shake.</summary>
	sf::Vector2<float> m_viewVelocity;
	float m_viewShakeTime;

	void shakeView(const sf::Time & p_time);

	void updateViewShake(const sf::Time & p_deltaTime);

	sf::IpAddress server_address;
	unsigned short server_port;

	std::string m_name;

	std::map<std::string, std::unique_ptr<Player>> m_players;

	std::vector<Projectile> m_projectiles;
	std::vector<Projectile>::iterator findID(const int & p_id);
	std::vector<int> m_eraseProjectileIDs;

	bool m_connected, m_active;

	Map m_map;

	sf::UdpSocket& m_socket;

	sf::Font gameFont;

	int m_elapsedGameTime; // this is a delta time, doesn't total elapsedGameTime
	int m_lastServerUpdateTime;
	sf::Clock timeSinceLastServerUpdate;
	sf::Time serverTimeout;

	sf::Text statusText;

	Scoreboard m_scoreboard;

	ParticleLoader m_particleLoader;
	ParticleEmitter m_particleEmitter;
};