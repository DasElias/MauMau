#pragma once
#include "ParticleSystem.h"

namespace card {
	class FireworkParticleSystem : public ParticleSystem {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ParticleTexture tex;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			FireworkParticleSystem(ParticleTexture tex);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void update(float delta, ParticleList& particleList) override;
			void explode(glm::vec2 center, ParticleList& particleList, float fireworkRadiusMultiplicator = 1.0f);
	};
}