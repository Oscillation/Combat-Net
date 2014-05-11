#pragma once
#include <iostream>
#include <map>
#include <dirent.h>
#include <vector>

#include "ParticleType.h"

class ParticleLoader{
public:
	ParticleLoader();
	ParticleLoader(const std::string & p_path);
	~ParticleLoader();

	void loadParticles(const std::string & p_path);

	ParticleType getParticleType(const std::string & p_name) const;

private:
	ParticleType loadParticle(const std::string & p_path);

	std::map<std::string, ParticleType> m_particleTypes;
};