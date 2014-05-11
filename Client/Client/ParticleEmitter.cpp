#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(){
	m_vertexArray.setPrimitiveType(sf::PrimitiveType::Lines);
}

ParticleEmitter::ParticleEmitter(ParticleLoader* ptr_particleLoader) : ptr_particleLoader(ptr_particleLoader){
	m_vertexArray.setPrimitiveType(sf::PrimitiveType::Lines);
}

ParticleEmitter::~ParticleEmitter(){

}

void ParticleEmitter::update(sf::Time & p_deltaTime){
	m_vertexArray.clear();
	for	(int i = 0; i < m_particles.size(); i++){
		if (m_particles[i].m_time <= 0)
		{
			if (m_particles[i].m_velocity.x > 0.2f || m_particles[i].m_velocity.y > 0.2f)
			{
				m_particles[i].m_velocity*=0.8f;

				m_particles[i].trail->m_velocity*=0.8f;

				m_particles[i].m_position += m_particles[i].m_velocity;

				m_particles[i].trail->m_position += m_particles[i].trail->m_velocity;

				m_vertexArray.append(m_particles[i].getVertex());
				m_vertexArray.append(m_particles[i].trail->getVertex());
			}else
			{
				if (m_particles[i].m_color.a > 10)
				{
					m_particles[i].m_velocity = (sf::Vector2<float>());
					m_particles[i].m_color.a *= 0.7f;

					m_particles[i].trail->m_velocity = sf::Vector2<float>();
					m_particles[i].trail->m_color.a *= 0.7f;

					m_vertexArray.append(m_particles[i].getVertex());
					m_vertexArray.append(m_particles[i].trail->getVertex());
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

			m_vertexArray.append(m_particles[i].getVertex());
			m_vertexArray.append(m_particles[i].trail->getVertex());
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

void ParticleEmitter::Emit(const std::string & p_type, const sf::Vector2<float> & p_position, const sf::Vector2<float> & p_velocity, const int & p_amount){
	for (int i = 0; i < p_amount; i++)
	{
		Particle* particle = new Particle(p_type, ptr_particleLoader->getParticleType(p_type), p_position, p_velocity);
		m_particles.push_back(*particle);
	}
}

void ParticleEmitter::Emit(const std::string & p_type, const sf::Vector2<float> & p_position, const int & p_amount){
	for (int i = 0; i < p_amount; i++)
	{
		sf::Vector2<float> velocity = sf::Vector2<float>((((float)math::random(1, 100))/100)*(math::random(-1, 1, 0)), (((float)math::random(1, 100))/100)*(math::random(-1, 1, 0)));
		Particle* particle = new Particle(p_type, ptr_particleLoader->getParticleType(p_type), p_position, velocity);
		m_particles.push_back(*particle);
	}
}

void ParticleEmitter::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const{
	p_target.draw(m_vertexArray);
}