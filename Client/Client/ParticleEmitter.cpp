#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(){
	
}

ParticleEmitter::~ParticleEmitter(){
	
}

void ParticleEmitter::Emit(const std::string & p_type, const ParticleType & p_particleType, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity){
	Particle particle = Particle(p_type, p_particleType, p_position, p_velocity);
}