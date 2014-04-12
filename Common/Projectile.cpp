#include "Projectile.h"

Projectile::Projectile(){
	
}

Projectile::~Projectile(){
	
}

sf::Vector2<float> Projectile::getVelocity() const{
	return m_velocity;
}

void Projectile::setVelocity(const sf::Vector2<float> & p_velocity){
	m_velocity = p_velocity;
}

void Projectile::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const{
	p_states.transform *= getTransform();
	sf::RectangleShape rect(sf::Vector2<float>(5, 5));
	rect.setFillColor(sf::Color::Blue);
	rect.setOrigin(2.5f, 2.5f);
	p_target.draw(rect, p_states);
}

sf::Packet& operator <<(sf::Packet& packet, const Projectile& projectile){
	return packet << projectile.getPosition().x << projectile.getPosition().y << projectile.getVelocity().x << projectile.getVelocity().y;
}

sf::Packet& operator >>(sf::Packet& packet, Projectile& projectile){
	sf::Vector2<float> pos, vel;
	packet >> pos.x >> pos.y >> vel.x >> vel.y;
	projectile.setPosition(pos);
	projectile.setVelocity(vel);
	return packet;
}