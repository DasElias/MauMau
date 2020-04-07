#pragma once
#include "Renderer3D.h"
#include "../model/CardAnimator.h"
#include "../renderingModel/SimpleTexture.h"
#include "CardRenderer.h"
#include "../renderingModel/PositionedCardStack.h"

namespace card {
	class DrawCardStackRenderer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static float const BORDER_RADIUS_WIDTH;
			static float const BORDER_RADIUS_HEIGHT;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			Renderer3D& renderer3D;
			CardRenderer& cardRenderer;
			ProjectionMatrix& projectionMatrix;
			Viewport& viewport;
			DataTextureVertexArrayObject borderVao;
			SimpleTexture texture;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			DrawCardStackRenderer(CardRenderer& cardRenderer, Renderer3D& renderer3D, ProjectionMatrix& pm, Viewport& vp);

		// ----------------------------------------------------------------------
		// ---------------------------STATIC-METHODS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::vector<float> genBorderData();
			static std::vector<float> genColorData(float amountOfCards);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void render(PositionedCardStack positionedCardStack, bool shouldRenderDisabled);
	};
}