#include "Projectile.h"

Projectile::Projectile() : erase(false){
}

Projectile::Projectile(const int & p_id) : m_id(p_id), erase(false){
}

Projectile::~Projectile(){

}

sf::Vector2<float> Projectile::getVelocity() const{
	return m_velocity;
}

void Projectile::setVelocity(const sf::Vector2<float> & p_velocity){
	m_velocity = p_velocity;
}

sf::String Projectile::getName() const{
	return m_name;
}

void Projectile::setName(const sf::String & p_name){
	m_name = p_name;
}

void Projectile::update(sf::Time p_deltaTime, int p_elapsedGameTime)
{
	move(getVelocity());

	float t = (float)(p_elapsedGameTime) / (float)(targetTime - prevTime);
	sf::Vector2i pos = (sf::Vector2i)math::interpolateVector(prevPos, targetPos, t);
	setPosition(sf::Vector2f(pos));
}

void Projectile::setTargetTime(int p_targetTime)
{
	prevTime = targetTime;
	targetTime = p_targetTime;
}

void Projectile::setTargetPosition(sf::Vector2f p_targetPosition)
{
	prevPos = targetPos;
	targetPos = p_targetPosition;
}

void Projectile::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const{
	p_states.transform *= getTransform();
	sf::RectangleShape rect(sf::Vector2<float>(5, 5));
	rect.setFillColor(sf::Color::Blue);
	rect.setOrigin(2.5f, 2.5f);
	p_target.draw(rect, p_states);
}