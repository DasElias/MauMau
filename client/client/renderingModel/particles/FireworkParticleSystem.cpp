#include "FireworkParticleSystem.h"
#include <shared/utils/MathUtils.h>
#include "../../utils/DimensionsConversionUtils.h"
#include <iostream>
#include <egui\input\IOHandler.h>

namespace card {
	FireworkParticleSystem::FireworkParticleSystem(ParticleTexture tex, int particlesPerExplosion, float lifeLengthSeconds, float velocity) :
			tex(tex),
			particlesPerExplosion(particlesPerExplosion),
			lifeLengthSeconds(lifeLengthSeconds),
			velocity(velocity) {
	}
	void FireworkParticleSystem::update(float delta, ParticleList& particleList) {
	}
	void FireworkParticleSystem::explode(glm::vec2 center, ParticleList& particleList, float fireworkRadiusMultiplicator) {
		float const ARC_PER_PARTICLE = 2*PI / particlesPerExplosion;

		float arcXZ = 0;
		for(int i = 0; i < particlesPerExplosion; i++) {
			
			
			float arcXY = 0;
			for(int j = 0; j < particlesPerExplosion/4; j++) {
				float velocityX = cos(arcXY) * cos(arcXZ) *  velocity;
				float velocityY = sin(arcXY) * cos(arcXZ) * velocity;
				float velocityZ = sin(arcXZ) * velocity;

				float endVelocityX = 0;
				float endVelocityY = 0;
				float endVelocityZ = 0;

				float const RANDOM_SCALE = 0.34f;
				float velocityAddition = RANDOM_SCALE * ((randomReal() * 2) - 1);
				velocityX += velocityAddition;
				velocityY += velocityAddition;
				velocityZ += velocityAddition;

				glm::vec3 velocity(velocityX, velocityY, velocityZ);
				velocity *= fireworkRadiusMultiplicator;
				glm::vec3 velocityAdditionPerSecond(endVelocityX - velocity.x, endVelocityY - velocity.y, endVelocityZ - velocity.z);
				velocityAdditionPerSecond /= lifeLengthSeconds;

				float rot = randomRealInRange<float>(0, 2*PI);
				Particle p({0,0,0}, velocity, velocityAdditionPerSecond, lifeLengthSeconds, rot, 0.075f, tex, center);
				particleList.add(p);

				arcXY += 2* ARC_PER_PARTICLE;
			}

			arcXZ += ARC_PER_PARTICLE;
		}
	}


	
}