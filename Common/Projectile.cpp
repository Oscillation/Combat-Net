#include "Projectile.h"

Projectile::Projectile() : 
	m_id(-1),
	m_updated(true)
{
	m_rectangleShape = sf::RectangleShape(sf::Vector2<float>(5, 5));
	m_rectangleShape.setFillColor(sf::Color(200, 200, 200));
	m_rectangleShape.setOrigin(2.5f, 2.5f);
}

Projectile::Projectile(const int & p_id, const int& p_damage, const sf::Vector2<float> & p_position) : 
	m_id(p_id),
	m_damage(p_damage),
	m_updated(true)
{
	setTargetPosition(p_position);
	m_rectangleShape = sf::RectangleShape(sf::Vector2<float>(5, 5));
	m_rectangleShape.setFillColor(sf::Color(200, 200, 200));
	m_rectangleShape.setOrigin(2.5f, 2.5f);
}

Projectile::~Projectile()
{

}

sf::Vector2<float> Projectile::getVelocity() const
{
	return m_velocity;
}

void Projectile::setVelocity(const sf::Vector2<float> & p_velocity)
{
	m_velocity = p_velocity;
}

std::string Projectile::getName() const
{
	return m_name;
}

void Projectile::setName(const std::string & p_name)
{
	m_name = p_name;
}

void Projectile::update(sf::Time p_deltaTime, int p_elapsedGameTime)
{
	move(getVelocity());
	
	if (m_targetPos != getPosition())
	{
		float t = (float)(p_elapsedGameTime) / (float)(m_targetTime - m_prevTime);
		sf::Vector2i pos = (sf::Vector2i)math::interpolateVector(m_prevPos, m_targetPos, t);
		setPosition(sf::Vector2f(pos));
		m_rectangleShape.setPosition(getPosition());
	}
}

void Projectile::setTargetTime(int p_targetTime)
{
	m_prevTime = m_targetTime;
	m_targetTime = p_targetTime;
}

void Projectile::setTargetPosition(sf::Vector2f p_targetPosition)
{
	m_prevPos = m_targetPos;
	m_targetPos = p_targetPosition;
}

void Projectile::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const
{
	//p_states.transform = getTransform();
	p_target.draw(m_rectangleShape);//, p_states);
}

bool Projectile::operator==(Projectile & p_projectile) const
{
	return m_id == p_projectile.m_id;
}

template <typename T>
sf::Packet& operator>>(sf::Packet & p_packet, sf::Vector2<T> & p_vec)
{
	return p_packet >> p_vec.x >> p_vec.y;
}

template <typename T>
sf::Packet& operator<<(sf::Packet & p_packet, const sf::Vector2<T> & p_vec)
{
	return p_packet << p_vec.x << p_vec.y;
}

sf::Packet& operator>>(sf::Packet & p_packet, Projectile & p_projectile)
{
	int id;
	sf::Vector2<float> pos, vel;

	p_packet >> id >> pos >> vel;

	p_projectile.m_id = id;
	p_projectile.setPosition(pos);
	p_projectile.setVelocity(vel);

	return p_packet;
}

sf::Packet& operator<<(sf::Packet & p_packet, Projectile & p_projectile)
{
	return p_packet << p_projectile.m_id << p_projectile.getPosition() << p_projectile.getVelocity();
}

sf::Packet& operator>>(sf::Packet & p_packet, std::vector<Projectile> & p_projectiles)
{
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

sf::Packet& operator<<(sf::Packet & p_packet, std::vector<Projectile> & p_projectiles)
{
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

Projectile::operator Object() const
{
	Object object = Object(ObjectType::Projectile, sf::Rect<float>(getPosition().x, getPosition().y, 5, 5), m_id);
	return object;
}