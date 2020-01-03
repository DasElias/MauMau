#pragma once

#include <glm/mat4x4.hpp>

namespace card {
	class ProjectionMatrix {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			float lastFov, lastAspectRatio;
			glm::mat4x4 projectionMatrix = glm::mat4x4(1);

			float zNear, zFar;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ProjectionMatrix(float fov, float zNear, float zFar);
			ProjectionMatrix(const ProjectionMatrix&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void update(float fov);
			glm::mat4x4 getProjectionMatrix() const;
	};
}
