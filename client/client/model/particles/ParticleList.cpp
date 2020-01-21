#include "ParticleList.h"
#include <algorithm>
#include <iostream>

namespace card {
	float const ParticleList::DENSITY = 0.05755f;
	float const ParticleList::MAX_DISTANCE = -log(0.5 / ((1 - 0.001) / (0.001 * 2))) / ParticleList::DENSITY;

	void ParticleList::update(float delta, glm::vec3 cameraPosition) {
		for(auto& particleType : particles) {
			std::vector<Particle>& particlesOfType = particleType.second;
			
			auto newEnd = std::remove_if(particlesOfType.begin(), particlesOfType.end(), [delta, cameraPosition](Particle& p) {
				bool isNotAlive = p.update(delta, cameraPosition);
				bool shouldRemove = isNotAlive || p.getDistanceFromCamera() > MAX_DISTANCE;
				return shouldRemove;
			});
			particlesOfType.erase(newEnd, particlesOfType.end());

			std::sort(particlesOfType.begin(), particlesOfType.end(), [](const Particle& a, const Particle& b) {
				return a.getDistanceFromCamera() < b.getDistanceFromCamera();
			});
		}

		removeParticleTexturesWithNoParticles();
	}
	
	void ParticleList::removeParticleTexturesWithNoParticles() {
		for(auto iter = particles.begin(); iter != particles.end(); ) {
			auto& particleList = (*iter).second;

			if(particleList.empty()) {
				iter = particles.erase(iter);
			} else {
				++iter;
			}
		}
	}
	void ParticleList::add(Particle p) {
		ParticleTexture particleTexture = p.getTexture();

		if(particles.find(particleTexture) != particles.end()) {
			particles.at(particleTexture).push_back(p);
		} else {
			std::vector<Particle> values;
			values.push_back(p);
			particles.insert(std::make_pair(particleTexture, values));
		}
	}
	ParticleList::iterator ParticleList::begin() {
		return particles.begin();
	}
	ParticleList::const_iterator ParticleList::begin() const {
		return particles.begin();
	}
	ParticleList::const_iterator ParticleList::cbegin() const {
		return particles.cbegin();
	}
	ParticleList::iterator ParticleList::end() {
		return particles.end();
	}
	ParticleList::const_iterator ParticleList::end() const {
		return particles.end();
	}
	ParticleList::const_iterator ParticleList::cend() const {
		return particles.cend();
	}
}