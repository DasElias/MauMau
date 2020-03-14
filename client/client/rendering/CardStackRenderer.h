#pragma once
#include "CardRenderer.h"
#include "../model/CardAnimator.h"
#include "../renderingModel/CardStackMisalignmentGenerator.h"


namespace card {
	class CardStackRenderer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static float const ADDITION_PER_CARD;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			CardRenderer& cardRendererImpl;
			CardStackMisalignmentGenerator& misalignmentGenerator;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CardStackRenderer(CardRenderer& cardRendererImpl, CardStackMisalignmentGenerator& misalignmentGenerator);
			CardStackRenderer(const CardStackRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void renderCardStack(const CardAnimator& cardStack, glm::vec3 position, glm::vec3 rotation, ProjectionMatrix& projectionMatrix, Viewport& viewport, bool shouldDisable = false);
			void renderCardStackWithMisalignment(const CardAnimator& cardStack, glm::vec3 position, glm::vec3 rotation, ProjectionMatrix& projectionMatrix, Viewport& viewport, bool shouldDisable = false);

	};
}