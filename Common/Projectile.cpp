#include "Projectile.h"

Projectile::Projectile() : erase(false), m_id(-1){
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

std::string Projectile::getName() const{
	return m_name;
}

void Projectile::setName(const std::string & p_name){
	m_name = p_name;
}

void Projectile::update(sf::Time p_deltaTime, int p_elapsedGameTime)
{
	move(getVelocity());
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
	sf::Vector2<float> pos, vel;

	p_packet >> id >> pos >> vel;

	p_projectile.m_id = id;
	p_projectile.setPosition(pos);
	p_projectile.setVelocity(vel);

	return p_packet;
}

sf::Packet& operator<<(sf::Packet & p_packet, Projectile & p_projectile){
	return p_packet << p_projectile.m_id << p_projectile.getPosition() << p_projectile.getVelocity();
}

sf::Packet& operator>>(sf::Packet & p_packet, std::vector<Projectile> & p_projectiles){
	unsigned int length;
	std::string name;
	p_packet >> length >> name;
	p_projectiles.resize(length, Projectile());
	for (int i = 0; i < length; i++)
	{
		p_packet >> p_projectiles[i];
		p_projectiles[i].setName(name);
	}
	return p_packet;
}

sf::Packet& operator<<(sf::Packet & p_packet, std::vector<Projectile> & p_projectiles){
	p_packet << p_projectiles.size();
	if (!p_projectiles.empty())
	{
		p_packet << p_projectiles.begin()->getName();
	}
	for (auto it = p_projectiles.begin(); it != p_projectiles.end(); ++it){
		p_packet << *it;
	}
	return p_packet;
}

