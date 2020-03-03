#pragma once
#include "ParticleSystem.h"

namespace card {
	class FireworkParticleSystem : public ParticleSystem {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ParticleTexture tex;
			int particlesPerExplosion;
			float lifeLengthSeconds;
			float velocity;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			FireworkParticleSystem(ParticleTexture tex, int particlesPerExplosion, float lifeLengthSeconds, float velocity);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void update(float delta, ParticleList& particleList) override;
			void explode(glm::vec2 center, ParticleList& particleList, float fireworkRadiusMultiplicator = 1.0f);
	};
}