#pragma once
#include <array>
#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include <egui/model\positioning\PositionableElement.h>
#include "../renderingModel/ProjectionMatrix.h"
#include "../renderingModel/Viewport.h"
#include "../renderingModel/DataTextureVertexArrayObject.h"

namespace card {
	class Renderer3D {
		class Renderer3DShader : public VertexFragmentShaderProgram {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				int location_projectionViewMatrix;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				Renderer3DShader(bool discardTransparentFragments);

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void loadProjectionViewMatrix(glm::mat4x4 projectionView);
		};




		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			Renderer3DShader shader;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Renderer3D(bool discardTransparentFragments = false);
			Renderer3D(const Renderer3D&) = delete;
			Renderer3D& operator=(const Renderer3D&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void render(const DataTextureVertexArrayObject& vertexArrayObject, const ProjectionMatrix& projectionMatrix, const Viewport& viewport, glm::mat4x4 modelMatrix = glm::mat4x4(1.0f));
		};
}