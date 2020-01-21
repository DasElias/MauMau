#pragma once
#include "ParticleList.h"

namespace card {
	class ParticleSystem {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ParticleSystem() = default;
			virtual ~ParticleSystem() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void update(float delta, ParticleList& particleList) =0;
	};
}