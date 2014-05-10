#pragma once
#include <SFML\Graphics\Color.hpp>
#include <SFML\Graphics\Vertex.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\System\Time.hpp>

#include "..\..\Common\GeneralMath.h"
#include "ParticleType.h"

struct Particle{
	Particle();
	Particle(const std::string & p_type, const ParticleType & p_particleType, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity);

	~Particle();

	void update(const sf::Time & p_deltaTime);

	sf::Color m_color;

	sf::Vector2<float> m_velocity;
	sf::Vector2<float> m_position;

	std::string m_type;

	sf::Vertex getVertex() const;

	///<summary>Seconds.</summary>
	float m_time;

	Particle* trail;
};