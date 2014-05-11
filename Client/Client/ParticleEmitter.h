#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <vector>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\VertexArray.hpp>
#include <SFML\System\Time.hpp>

#include "Particle.h"
#include "ParticleType.h"
#include "ParticleLoader.h"

class ParticleEmitter : public sf::Drawable{
public:
	ParticleEmitter();
	ParticleEmitter(ParticleLoader* ptr_particleLoader);
	~ParticleEmitter();

	///<summary>p_velocity = % of the speed of the particle type in the given direction.</summary>
	void Emit(const std::string & p_type, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity);

	void Emit(const std::string & p_type, const sf::Vector2<float> & p_position);

	///<summary>p_velocity = % of the speed of the particle type in the given direction.</summary>
	void Emit(const std::string & p_type, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity, const int & p_amount);

	void Emit(const std::string & p_type, const sf::Vector2<float> & p_position, const int & p_amount);

	void update(sf::Time & p_deltaTime);

	void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;

private:
	std::vector<Particle> m_particles;
	ParticleLoader* ptr_particleLoader;

	sf::VertexArray m_vertexArray;
};