#pragma once
#include "CardRenderer.h"
#include "../model/CardAnimator.h"

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

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CardStackRenderer(CardRenderer& cardRendererImpl);
			CardStackRenderer(const CardStackRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void renderCardStack(const CardAnimator& cardStack, glm::vec3 position, glm::vec3 rotation, ProjectionMatrix& projectionMatrix, Viewport& viewport);
	};
}