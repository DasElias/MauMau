#pragma once
#include "../model/CardAnimator.h"
#include <glm/glm.hpp>
#include "IntersectionChecker.h"

namespace card {
	class CardStackIntersectionChecker : public IntersectionChecker {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CardStackIntersectionChecker(ProjectionMatrix& projectionMatrix, Viewport& viewport);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool doesIntersect(const CardAnimator& cardStack, glm::vec3 centerPosition, glm::vec3 rotation, float heightAdditionPerCard, float widthOfSingleCard, float heightOfSingleCard);

		private:
			std::array<glm::vec2, 4> generateScreenPositions(std::array<glm::vec4, 4>& positionsInLocalSpace, glm::vec3 centerPosition, glm::vec3 rotation);

	};
}