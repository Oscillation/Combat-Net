#include "Player.h"

#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

#include "..\..\Common\GeneralMath.h"

#include <iostream>

Player::Player(sf::String p_name, sf::Font& p_font, bool p_remote)
	: 
	m_remote(p_remote),
	nameText(p_name, p_font, 14),
	m_radius(20),
	m_dead(false),
	m_health(1)
{
	nameText.setPosition(-nameText.getLocalBounds().width/2, 24);
	nameText.setStyle(sf::Text::Bold);
	nameText.setColor(sf::Color::Black);
}

Player::~Player()
{

}

void Player::update(sf::Time p_deltaTime, int p_elapsedGameTime)
{
	float t = (float)(p_elapsedGameTime) / (float)(targetTime - prevTime);
	sf::Vector2i pos = (sf::Vector2i)math::interpolateVector(prevPos, targetPos, t);
	setPosition(sf::Vector2f(pos));
}

void Player::setTargetTime(int p_targetTime)
{
	prevTime = targetTime;
	targetTime = p_targetTime;
}

void Player::setTargetPosition(sf::Vector2f p_targetPosition)
{
	prevPos = targetPos;
	targetPos = p_targetPosition;
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

int Player::getHealth() const{
	return m_health;
}

void Player::setHealth(const int & p_health){
	m_health = p_health;
	if (m_health <= 0)
	{
		m_dead = true;
	}
}

bool Player::isDead() const{
	return m_dead;
}