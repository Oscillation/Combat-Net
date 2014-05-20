#include "PreMatchState.h"

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Network\UdpSocket.hpp>
#include <SFML\Network\Packet.hpp>

#include "..\..\Common\Protocol.h"

PreMatchState::PreMatchState(StateStack& stateStack, Context& context, States::ID id)
	:
	State(stateStack, context, id)
{
	m_statusText.setFont(*context.font);
	m_statusText.setColor(sf::Color::Red);
	m_statusText.setPosition(100, 100);
	m_statusText.setString("Waiting for match...");
}

PreMatchState::~PreMatchState()
{
}

bool PreMatchState::update(sf::Time & p_deltaTime)
{
	sf::Packet packet;
	sf::IpAddress address;
	unsigned short port;
	while (getContext()->socket->receive(packet, address, port) == sf::Socket::Done)
	{
		int type, time;
		packet >> time >> type;

		if ((cn::PacketType)type == cn::MatchStart)
		{
			requestStackPop();
		}
		else if ((cn::PacketType)type == cn::Ping)
		{
			sf::Packet pingPacket;
			pingPacket << 0 << cn::Ping << m_name;
			m_socket.send(pingPacket, address, port);
		}

	}
	return false;
}

bool PreMatchState::draw()
{
	getContext()->window->clear(sf::Color::Black);
	getContext()->window->draw(m_statusText);
	return false;
}

bool PreMatchState::handleEvents(const sf::Event& event)
{
	return false;
}