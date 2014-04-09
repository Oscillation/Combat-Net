#include "Player.h"

#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

Player::Player(bool p_remote)
	: 
	m_remote(p_remote)
{
}

Player::~Player()
{

}

bool Player::isRemote() const
{
	return m_remote;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::CircleShape shape(20);
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(getPosition());
	target.draw(shape);
}