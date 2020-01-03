#pragma once

#include "ProjectionMatrix.h"
#include "Viewport.h"

namespace card {
	class IntersectionChecker {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			ProjectionMatrix& projectionMatrix;
			Viewport& viewport;
			
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			IntersectionChecker(ProjectionMatrix& projectionMatrix, Viewport& viewport);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			bool isPointInTriangle(glm::vec2 point, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
			float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);

	};
}