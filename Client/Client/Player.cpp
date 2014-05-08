#include "Player.h"

#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

#include "..\..\Common\GeneralMath.h"

#include <iostream>

Player::Player(std::string p_name, sf::Font& p_font, bool p_remote)
	: 
	m_remote(p_remote),
	nameText(p_name, p_font, 14),
	m_radius(20),
	m_dead(false),
	m_health(100),
	m_score()
{
	std::cout << "Called constructor for: " << p_name << "\n";
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
	
	sf::CircleShape player(m_radius);
	player.setFillColor(sf::Color::Red);
	player.setOrigin(m_radius, m_radius);

	sf::RectangleShape healthBackground(sf::Vector2<float>(40.f, 5));
	sf::RectangleShape health(sf::Vector2<float>(40.f*((float)getHealth()/100), 5));

	healthBackground.setFillColor(sf::Color(20, 20, 20));
	health.setFillColor(sf::Color(200, 200, 200));

	healthBackground.setOrigin(m_radius, m_radius + 10);
	health.setOrigin(m_radius, m_radius + 10);

	target.draw(player, states);
	target.draw(healthBackground, states);
	target.draw(health, states);
	target.draw(nameText, states);
}

int Player::getHealth() const{
	return m_health;
}

void Player::setHealth(const int & p_health){
	m_health = p_health;
	m_dead = (m_health <= 0);
}

bool Player::isDead() const{
	return m_dead;
}