#include "Projectile.h"

Projectile::Projectile() : erase(false), m_id(-1){
}

Projectile::Projectile(const int & p_id, const int& p_damage) : 
	m_id(p_id), m_damage(p_damage), erase(false){
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
	rect.setFillColor(sf::Color(200, 200, 200));
	rect.setOrigin(2.5f, 2.5f);
	p_target.draw(rect, p_states);
}

bool Projectile::operator==(Projectile & p_projectile) const{
	return m_id == p_projectile.m_id;
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
	p_packet >> length;
	if (length > 0)//See: operator<<(sf::Packet & p_packet, std::vector<Projectile> & p_projectiles)
	{
		std::string name;
		p_packet >> name;
		for (unsigned int i = 0; i < length; i++)
		{
			Projectile projectile = Projectile();
			p_packet >> projectile;
			projectile.setName(name);
			p_projectiles.push_back(projectile);
		}
	}
	return p_packet;
}

sf::Packet& operator<<(sf::Packet & p_packet, std::vector<Projectile> & p_projectiles){
	if (!p_projectiles.empty())
	{
		p_packet << p_projectiles.size() << p_projectiles.begin()->getName();
		for (auto it = p_projectiles.begin(); it != p_projectiles.end(); ++it){
			p_packet << *it;
		}
	}else
	{
		p_packet << 0;//See: operator>>(sf::Packet & p_packet, std::vector<Projectile> & p_projectiles)
	}
	return p_packet;
}

