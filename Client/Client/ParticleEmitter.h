#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <vector>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\VertexArray.hpp>
#include <SFML\System\Time.hpp>

#include "Particle.h"
#include "ParticleType.h"

class ParticleEmitter : sf::Drawable{
public:
	ParticleEmitter();
	~ParticleEmitter();

	///<summary>p_velocity = % of the speed of the particle type in the given direction.</summary>
	void Emit(const std::string & p_type, const ParticleType & p_particleType, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity);

	void update(sf::Time & p_deltaTime);

	void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;

private:
	std::vector<Particle> m_particles;
	sf::VertexArray m_vertexArray;
};