#include "Particle.h"

Particle::Particle(const std::string & p_type, const ParticleType & p_particleType, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity) : 
	m_type(p_type),
	m_position(p_position),
	m_velocity(p_particleType.getSpeed()*p_velocity.x + math::random(p_particleType.getSpeedOffset().x, p_particleType.getSpeedOffset().y), p_particleType.getSpeed()*p_velocity.y + math::random(p_particleType.getSpeedOffset().x, p_particleType.getSpeedOffset().y)),
	m_color(sf::Color(math::random(p_particleType.getRRange().x, p_particleType.getRRange().y),
	math::random(p_particleType.getGRange().x, p_particleType.getGRange().y),
	math::random(p_particleType.getBRange().x, p_particleType.getBRange().y))),
	m_time(((float)(math::random((int)(p_particleType.getTimeRange().x*100), (int)(p_particleType.getTimeRange().y*100))))/100)
{
	trail = new Particle(*this);
	trail->m_velocity = (sf::Vector2<float>(m_velocity.x*0.9f, m_velocity.y*0.9f));
}

Particle::Particle(){

}

Particle::~Particle(){

}

bool Particle::update(const sf::Time & p_deltaTime){

	if (m_time > 0)
	{
		m_time -= p_deltaTime.asSeconds();
		m_position += m_velocity;
		trail->m_position += trail->m_velocity;
	}else
	{
		if (std::abs(m_velocity.x) > 0.2f)
		{
			m_velocity *= 0.8f;
			m_position += m_velocity;
			trail->m_velocity *= 0.8f;
			trail->m_position += trail->m_velocity;
		}else if (m_color.a > 10)
		{
			m_color.a *= 0.8f;
			trail->m_color.a *= 0.8f;
		}else
		{
			return false;
		}
	}
	return true;
}

sf::Vertex Particle::getVertex() const{ 
	return sf::Vertex(m_position, m_color);
}