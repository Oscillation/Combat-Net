#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(){

}

ParticleEmitter::ParticleEmitter(ParticleLoader* ptr_particleLoader) : ptr_particleLoader(ptr_particleLoader){

}

ParticleEmitter::~ParticleEmitter(){

}

void ParticleEmitter::update(sf::Time & p_deltaTime){
	for	(int i = 0; i < m_particles.size();){
		if (m_particles[i].m_time <= 0)
		{
			if (m_particles[i].m_velocity.x > 0.1f || m_particles[i].m_velocity.y > 0.1f)
			{
				m_particles[i].m_velocity*=0.8f;

				m_particles[i].trail->m_velocity*=0.8f;

				i++;
			}else
			{
				if (m_particles[i].m_color.a > 10)
				{
					m_particles[i].m_velocity = (sf::Vector2<float>());
					m_particles[i].m_color.a *= 0.7f;

					m_particles[i].trail->m_velocity = sf::Vector2<float>();
					m_particles[i].trail->m_color.a *= 0.7f;

					i++;
				}else
				{
					m_particles.erase(m_particles.begin() + i);
				}
			}
		}else
		{
			m_particles[i].update(p_deltaTime);
			m_particles[i].m_position += m_particles[i].m_velocity;

			m_particles[i].trail->m_position += m_particles[i].trail->m_velocity;

			i++;
		}
	}
}

void ParticleEmitter::Emit(const std::string & p_type, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity){
	Particle* particle = new Particle(p_type, ptr_particleLoader->getParticleType(p_type), p_position, p_velocity);
	m_particles.push_back(*particle);
}

void ParticleEmitter::Emit(const std::string & p_type, const sf::Vector2<float> & p_position){
	sf::Vector2<float> velocity = sf::Vector2<float>((((float)math::random(1, 100))/100)*(math::random(-1, 1, 0)), (((float)math::random(1, 100))/100)*(math::random(-1, 1, 0)));
	Particle* particle = new Particle(p_type, ptr_particleLoader->getParticleType(p_type), p_position, velocity);
	m_particles.push_back(*particle);
}

void ParticleEmitter::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const{
	sf::VertexArray vertexArray;
	vertexArray.setPrimitiveType(sf::PrimitiveType::Lines);

	for	(auto it = m_particles.begin(); it != m_particles.end(); ++it){
		vertexArray.append(it->getVertex());
		vertexArray.append(it->trail->getVertex());
	}

	p_target.draw(vertexArray);
}