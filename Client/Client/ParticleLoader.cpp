#include "ParticleLoader.h"

ParticleLoader::ParticleLoader(){

}

ParticleLoader::ParticleLoader(const std::string & p_path){
	loadParticles(p_path);
}

ParticleLoader::~ParticleLoader(){

}

void ParticleLoader::loadParticles(const std::string & p_path){
	std::vector<std::string> files;

	//Get files in directory
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (p_path.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			files.push_back(ent->d_name);
		}
		closedir (dir);
	}

	for (int i = 2; i < files.size(); i++)
	{
		m_particleTypes[files[i].substr(0, files[i].length() - 4)] = loadParticle(p_path + files[i]);
	}
}

ParticleType ParticleLoader::loadParticle(const std::string & p_path){
	ParticleType particle = ParticleType(p_path);
	return particle;
}

ParticleType ParticleLoader::getParticleType(const std::string & p_name) const{
	return m_particleTypes.at(p_name);
}