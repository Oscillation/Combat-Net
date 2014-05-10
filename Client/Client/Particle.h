#pragma once
#include <SFML\Graphics\Color.hpp>
#include "..\..\Common\GeneralMath.h"

#include "ParticleType.h"

struct Particle{
	Particle();
	Particle(const std::string & p_type, const ParticleType & p_particleType, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity);

	~Particle();

	sf::Color m_color;

	short m_size;

	sf::Vector2<float> m_velocity;
	sf::Vector2<float> m_position;

	std::string m_type;
};