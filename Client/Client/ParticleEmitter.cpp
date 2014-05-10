#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(){
	
}

ParticleEmitter::~ParticleEmitter(){
	
}

void ParticleEmitter::update(sf::Time & p_deltaTime){
	for	(auto it = m_particles.begin(); it != m_particles.end(); ++it){
		
	}
}

void ParticleEmitter::Emit(const std::string & p_type, const ParticleType & p_particleType, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity){
	Particle particle = Particle(p_type, p_particleType, p_position, p_velocity);
}

void ParticleEmitter::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const{

}