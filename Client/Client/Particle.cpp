#include "Particle.h"

Particle::Particle(const std::string & p_type, const ParticleType & p_particleType, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity) : 
	m_type(p_type),
	m_position(p_position),
	m_velocity(p_particleType.getSpeed()*p_velocity.x + math::random(p_particleType.getSpeedOffset().x, p_particleType.getSpeedOffset().y), p_particleType.getSpeed()*p_velocity.y + math::random(p_particleType.getSpeedOffset().x, p_particleType.getSpeedOffset().y)),
	m_color(sf::Color(math::random(p_particleType.getRRange().x, p_particleType.getRRange().y),
	math::random(p_particleType.getGRange().x, p_particleType.getGRange().y),
	math::random(p_particleType.getBRange().x, p_particleType.getBRange().y))),
	m_size(math::random(p_particleType.getSizeRange().x, p_particleType.getSizeRange().y))
{
	
}

Particle::Particle(){
	
}

Particle::~Particle(){
	
}

Particle::operator sf::Vertex(){
	sf::CircleShape circle(m_size);

	return ;
}