#include "FireworkParticleSystem.h"
#include <shared/utils/MathUtils.h>
#include "../../utils/DimensionsConversionUtils.h"
#include <iostream>
#include <egui\input\IOHandler.h>

namespace card {
	FireworkParticleSystem::FireworkParticleSystem(ParticleTexture tex) :
			tex(tex) {
	}
	void FireworkParticleSystem::update(float delta, ParticleList& particleList) {
	}
	void FireworkParticleSystem::explode(glm::vec2 center, ParticleList& particleList) {
		int const PARTICLES_PER_EXPLOSION = 20;
		float const ARC_PER_PARTICLE = 2*PI / PARTICLES_PER_EXPLOSION;

		float const LIFE_LENGTH_SECONDS = 1.5f;
		float const VELOCITY_SCALE = 2.0f;

		float arcXZ = 0;
		for(int i = 0; i < PARTICLES_PER_EXPLOSION; i++) {
			
			
			float arcXY = 0;
			for(int j = 0; j < PARTICLES_PER_EXPLOSION/4; j++) {
				float velocityX = cos(arcXY) * cos(arcXZ) *  VELOCITY_SCALE;
				float velocityY = sin(arcXY) * cos(arcXZ) * VELOCITY_SCALE;
				float velocityZ = sin(arcXZ) * VELOCITY_SCALE;

				float endVelocityX = 0;
				float endVelocityY = 0;
				float endVelocityZ = 0;

				float const RANDOM_SCALE = 0.34f;
				float velocityXAddition = RANDOM_SCALE * ((randomReal() * 2) - 1);
				velocityX += velocityXAddition;
				velocityY += velocityXAddition;
				velocityZ += velocityXAddition;

				glm::vec3 velocity(velocityX, velocityY, velocityZ);
				glm::vec3 velocityAdditionPerSecond(endVelocityX - velocityX, endVelocityY - velocityY, endVelocityZ - velocityZ);
				velocityAdditionPerSecond /= LIFE_LENGTH_SECONDS;


				Particle p({0,0,0}, velocity, velocityAdditionPerSecond, LIFE_LENGTH_SECONDS, 0, 0.05f, tex, center);
				particleList.add(p);

				arcXY += 2* ARC_PER_PARTICLE;
			}

			arcXZ += ARC_PER_PARTICLE;
		}
	}


	
}