#pragma once
#include "CardRenderer.h"
#include <shared/model/HandCardStack.h>
#include "../renderingModel/HandCardStackPositionGenerator.h"

namespace card {
	class HandCardStackRenderer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static float const CARD_OVERLAPPING_FACTOR;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			CardRenderer& cardRendererImpl;
			HandCardStackPositionGenerator positionGenerator;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			HandCardStackRenderer(CardRenderer& cardRendererImpl);
			HandCardStackRenderer(const HandCardStackRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void renderCardStackInX(const CardAnimator& cardStack, glm::vec3 centerPosition, glm::vec3 rotation, ProjectionMatrix& projectionMatrix, Viewport& viewport, float maxWidth, int selectedCardIndex = -1, float selectedCardAddition = 0);
			void renderCardStackInZ(const CardAnimator& cardStack, glm::vec3 centerPosition, glm::vec3 rotation, ProjectionMatrix& projectionMatrix, Viewport& viewport, float maxWidth, int selectedCardIndex = -1, float selectedCardAddition = 0);
	};
}