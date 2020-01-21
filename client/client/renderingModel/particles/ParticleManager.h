#pragma once

#include "ParticleSystem.h"
#include <memory>
#include "ParticleList.h"
#include <cstdint>
#include <map>
#include <memory>
namespace card {
	typedef uint64_t ParticleSystemIdentification;
	
	class ParticleManager {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::map<ParticleSystemIdentification, std::shared_ptr<ParticleSystem>> particleSystems;
			ParticleList particleList;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ParticleManager() = default;
			ParticleManager(const ParticleManager&) = delete;


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			ParticleSystemIdentification add(std::shared_ptr<ParticleSystem> particleSystem);
			void remove(ParticleSystemIdentification particleSystemIdentification);
			void update(float delta, glm::vec3 cameraPosition);
			const ParticleList& getList() const;
			ParticleList& getList();

		private:
			ParticleSystemIdentification genUniqueParticleSystemIdentification();
	};

}
