#include "Player.h"

#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

Player::Player(sf::String p_name, sf::Font& p_font, bool p_remote)
	: 
	m_remote(p_remote),
	nameText(p_name, p_font, 14),
	m_radius(20)
{
	nameText.setPosition(-nameText.getLocalBounds().width/2, 24);
	nameText.setStyle(sf::Text::Bold);
	nameText.setColor(sf::Color::Black);
}

Player::~Player()
{

}

bool Player::isRemote() const
{
	return m_remote;
}

float Player::getRadius() const{
	return m_radius;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	sf::CircleShape shape(m_radius);
	shape.setFillColor(sf::Color::Red);
	shape.setOrigin(m_radius, m_radius);
	target.draw(shape, states);
	target.draw(nameText, states);
}