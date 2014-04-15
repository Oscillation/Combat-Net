#pragma once
#include <SFML\Network\IpAddress.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <vector>
#include <memory>
#include "..\..\Common\Projectile.h"

struct Client : public sf::Transformable {
	Client();
	Client(const sf::IpAddress & p_address, const unsigned short & p_port);
	~Client();

	sf::IpAddress getAddress();

	unsigned short getPort();

	bool hasRespondedToPing;

	void setSpeed(float p_speed);
	float getSpeed() const;

private:
	sf::IpAddress m_address;
	unsigned short m_port;

	float m_speed;
};