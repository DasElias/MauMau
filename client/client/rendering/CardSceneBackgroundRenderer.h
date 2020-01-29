#pragma once
#include "Renderer2D.h"
#include "Renderer3D.h"

#include <egui/model/nodes/UnorganizedParentElement.h>
#include "../renderingModel/SimpleTexture.h"
#include "../renderingModel/DataTextureVertexArrayObject.h"
#include "../renderingModel/DataTextureNormalVertexArrayObject.h"

namespace card {
	class CardSceneBackgroundRenderer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::vector<float> const TABLE_VERTICES;
			static std::vector<float> const TABLE_TEXTURE_COORDS;
			static std::vector<float> const TABLE_NORMALS;

		public:
			static float const TABLE_MAX_X;
			static float const TABLE_MAX_Z;
			
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			Renderer2D& renderImpl2d;
			Renderer3D& renderImpl3d;
			DataTextureNormalVertexArrayObject tableVao;

			std::shared_ptr<egui::UnorganizedParentElement> backgroundElement;
			SimpleTexture backgroundTexture;
			SimpleTexture tableTopTexture;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CardSceneBackgroundRenderer(Renderer2D& rendererImpl2d, Renderer3D& renderImpl3d);
			CardSceneBackgroundRenderer(const CardSceneBackgroundRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void render(ProjectionMatrix& projectionMatrix, Viewport& viewport);


	};
}