#pragma once
#include <SFML\Network\IpAddress.hpp>
#include <SFML\System\Vector2.hpp>

struct Client{
	Client();
	Client(const sf::IpAddress & p_address, const unsigned short & p_port);
	~Client();

	sf::IpAddress getAddress();

	unsigned short getPort();

	sf::Vector2<float> getPosition() const;

	void setPosition(const sf::Vector2f & p_position);

	bool hasRespondedToPing;

private:
	sf::IpAddress m_address;
	unsigned short m_port;

	sf::Vector2<float> m_position;
};