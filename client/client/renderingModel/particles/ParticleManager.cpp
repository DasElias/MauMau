#include "ParticleManager.h"

namespace card {
	ParticleSystemIdentification ParticleManager::add(std::shared_ptr<ParticleSystem> ps) {
		ParticleSystemIdentification id = genUniqueParticleSystemIdentification();
		particleSystems.insert(std::make_pair(id, std::move(ps)));
		return id;
	}
	void ParticleManager::remove(ParticleSystemIdentification particleSystemIdentification) {
		particleSystems.erase(particleSystemIdentification);
	}
	void ParticleManager::update(float delta, glm::vec3 cameraPosition) {
		for(auto& ps : particleSystems) {
			ps.second->update(delta, particleList);
		}

		particleList.update(delta, cameraPosition);
	}
	const ParticleList& ParticleManager::getList() const {
		return particleList;
	}
	ParticleList& ParticleManager::getList() {
		return particleList;
	}
	ParticleSystemIdentification ParticleManager::genUniqueParticleSystemIdentification() {
		static ParticleSystemIdentification ids = 0;
		return ids++;
	}
}