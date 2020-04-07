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
			ProjectionMatrix& projectionMatrix;
			Viewport& viewport;
			HandCardStackPositionGenerator positionGenerator;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			HandCardStackRenderer(CardRenderer& cardRendererImpl, ProjectionMatrix& projectionMatrix, Viewport& viewport);
			HandCardStackRenderer(const HandCardStackRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void renderCardStackInX(PositionedCardStack positionedCardStack, float maxWidth, int selectedCardIndex = -1, float selectedCardAddition = 0, std::vector<bool> shouldRenderInGreyScale = {});
			void renderCardStackInZ(PositionedCardStack positionedCardStack, float maxWidth, int selectedCardIndex = -1, float selectedCardAddition = 0, std::vector<bool> shouldRenderInGreyScale = {});
	};
}