#pragma once
#include <SFML\Network\IpAddress.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\System\Clock.hpp>
#include <vector>
#include <memory>

#include "..\..\Common\Projectile.h"
#include "..\..\Common\Protocol.h"
#include "..\..\Common\Score.h"

struct Client : public sf::Transformable {
	Client();
	Client(const sf::IpAddress & p_address, const unsigned short & p_port);
	~Client();

	sf::IpAddress getAddress();

	unsigned short getPort();

	bool hasRespondedToPing;

	void setSpeed(float p_speed);
	float getSpeed() const;

	void setName(const std::string & p_name);
	std::string getName() const;

	bool shoot();

	Score m_score;

	bool operator==(Client & p_client) const;

	int getHealth() const;
	void setHealth(const int& p_health);
	void damage(const int& p_damage);

	int getDamage() const;
	void setDamage(const int& p_damage);

	///<summary>Seconds.</summary>
	float m_respawnTime;

private:
	int m_health;

	int m_damage;

	sf::IpAddress m_address;
	unsigned short m_port;

	std::string m_name;

	float m_speed;

	sf::Time m_shootDelay;
	sf::Clock m_lastShot;
};