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
		m_particleTypes[files[i]] = loadParticle(p_path + files[i]);
	}
}

ParticleType & ParticleLoader::loadParticle(const std::string & p_path){
	return ParticleType(p_path);
}