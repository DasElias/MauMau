#pragma once
#include <glm/glm.hpp>

namespace card {
	class Viewport {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			glm::vec3 position;
			glm::vec3 rotation;
			float scale;

			glm::mat4x4 viewMatrix;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Viewport(glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), float scale = 1);
			Viewport(const Viewport&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void setPosition(glm::vec3 pos);
			void setRotation(glm::vec3 rotation);
			void setScale(float scale);

			glm::vec3 getPosition() const;
			glm::vec3 getRotation() const;
			float getScale() const;
			glm::mat4x4 getViewMatrix() const;

		private:
			void update();

	};
}