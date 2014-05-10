#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <vector>

#include "Particle.h"
#include "ParticleType.h"

class ParticleEmitter : sf::Drawable{
public:
	ParticleEmitter();
	~ParticleEmitter();

private:
	///<summary>p_velocity = % of the speed of the particle type in the given direction.</summary>
	void Emit(const std::string & p_type, const ParticleType & p_particleType, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity);

	std::vector<Particle> m_particles;
};