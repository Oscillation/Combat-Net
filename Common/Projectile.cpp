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

template <typename T>
sf::Packet& operator>>(sf::Packet & p_packet, sf::Vector2<T> & p_vec){
	return p_packet >> p_vec.x >> p_vec.y;
}

template <typename T>
sf::Packet& operator<<(sf::Packet & p_packet, const sf::Vector2<T> & p_vec){
	return p_packet << p_vec.x << p_vec.y;
}

sf::Packet& operator>>(sf::Packet & p_packet, Projectile & p_projectile){
	int id;
	sf::String name;
	sf::Vector2<float> pos, vel;

	p_packet >> id >> name >> pos >> vel;

	p_projectile.m_id = id;
	p_projectile.setName(name);
	p_projectile.setPosition(pos);
	p_projectile.setVelocity(vel);

	return p_packet;
}

sf::Packet& operator<<(sf::Packet & p_packet, Projectile & p_projectile){
	if (p_projectile.erase)
	{
		return p_packet;
	}else
	{
		return p_packet << p_projectile.m_id << p_projectile.getName() << p_projectile.getPosition() << p_projectile.getVelocity();
	}
}

sf::Packet& operator>>(sf::Packet & p_packet, std::vector<Projectile> & p_projectiles){
	unsigned int length;
	p_packet >> length;
	p_projectiles.resize(length, Projectile());
	for (int i = 0; i < length; i++)
	{
		p_packet >> p_projectiles[i];
	}
	return p_packet;
}

sf::Packet& operator<<(sf::Packet & p_packet, std::vector<Projectile> & p_projectiles){
	p_packet << p_projectiles.size();
	for (auto it = p_projectiles.begin(); it != p_projectiles.end(); ++it){
		p_packet << *it;
	}
	return p_packet;
}

