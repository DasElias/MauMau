#pragma once
#include <glm/glm.hpp>
#include "ParticleTexture.h"

namespace card {
	class Particle {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			glm::vec3 position;
			glm::vec3 velocity;
			glm::vec3 velocityAdditionPerSecond;
			float lifeLength;
			float rotation;
			float scale;
			float elapsedTime;

			ParticleTexture texture;

			glm::vec2 offsetTex1 = glm::vec3(0.0);
			glm::vec2 offsetTex2 = glm::vec3(0.0);
			float texBlendFactor = 0;

			float distanceFromCamera = 0;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Particle(glm::vec3 position, glm::vec3 startVelocity, glm::vec3 velocityAdditionPerSecond, float lifeLengthSeconds, float rotation, float scale, ParticleTexture texture);
		//	Particle(const Particle&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			// returns true if the particle isn't alive any more
			bool update(float delta, glm::vec3 playerPosition);
			glm::vec3 getPosition() const;
			void setPosition(glm::vec3 position);
			float getRotation() const;
			float getScale() const;
			ParticleTexture getTexture() const;
			glm::vec2 getOffsetTex1() const;
			glm::vec2 getOffsetTex2() const;
			float getTexBlendFactor() const;
			float getDistanceFromCamera() const;
			glm::mat4x4 generateModelMatrix(glm::mat4x4 viewMatrix) const;

		private:
			void updateTex(float delta);
			glm::vec2 getTexOffset(int index);
	};
}