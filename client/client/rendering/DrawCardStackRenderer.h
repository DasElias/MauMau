#pragma once
#include "Renderer3D.h"
#include "../model/CardAnimator.h"
#include "../renderingModel/SimpleTexture.h"
#include "CardRenderer.h"

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
			DataTextureVertexArrayObject borderVao;
			SimpleTexture texture;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			DrawCardStackRenderer(CardRenderer& cardRenderer, Renderer3D& renderer3D);

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
			void render(const CardAnimator& cardStack, glm::vec3 position, glm::vec3 rotation, ProjectionMatrix& projectionMatrix, Viewport& viewport);
	};
}