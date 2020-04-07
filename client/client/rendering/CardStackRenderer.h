#pragma once
#include "CardRenderer.h"
#include "../model/CardAnimator.h"
#include "../renderingModel/CardStackMisalignmentGenerator.h"
#include "../renderingModel/PositionedCardStack.h"


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
			ProjectionMatrix& projectionMatrix;
			Viewport& viewport;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CardStackRenderer(CardRenderer& cardRendererImpl, CardStackMisalignmentGenerator& misalignmentGenerator, ProjectionMatrix& pm, Viewport& vp);
			CardStackRenderer(const CardStackRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void renderCardStack(PositionedCardStack positionedCardStack, bool shouldDisable = false);
			void renderCardStackWithMisalignment(PositionedCardStack positionedCardStack, bool shouldDisable = false);

	};
}