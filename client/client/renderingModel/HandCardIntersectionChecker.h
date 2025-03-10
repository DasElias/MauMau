#pragma once
#include <optional>
#include "PositionedCard.h"
#include <shared/model/HandCardStack.h>
#include "HandCardStackPositionGenerator.h"
#include "ProjectionMatrix.h"
#include "Viewport.h"
#include <optional>
#include "IntersectionChecker.h"

namespace card {
	class HandCardIntersectionChecker : public IntersectionChecker {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			HandCardStackPositionGenerator positionGenerator;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			HandCardIntersectionChecker(ProjectionMatrix& projectionMatrix, Viewport& viewport);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::optional<int> getIndexOfIntersectedCardInX(PositionedCardStack positionedCardStack, CardDimensions cardDimensions, float maxWidthOfCardStack);

		private:
			bool isCursorOutsideOfScreen();
	};
}