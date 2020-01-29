#pragma once
#include <array>
#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include <egui/model\positioning\PositionableElement.h>
#include "../renderingModel/ProjectionMatrix.h"
#include "../renderingModel/Viewport.h"
#include "../renderingModel/DataTextureVertexArrayObject.h"
#include "../renderingModel/DataTextureNormalVertexArrayObject.h"

namespace card {
	class TableRenderer {
		class TableRendererShader : public VertexFragmentShaderProgram {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				int location_projectionViewMatrix;
				int location_modelMatrix;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				TableRendererShader();

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void loadProjectionViewMatrix(glm::mat4x4 projectionView);
				void loadModelMatrix(glm::mat4 modelMatrix);
		};




		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			TableRendererShader shader;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			TableRenderer();
			TableRenderer(const TableRenderer&) = delete;
			TableRenderer& operator=(const TableRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void render(const DataTextureNormalVertexArrayObject& vertexArrayObject, const ProjectionMatrix& projectionMatrix, const Viewport& viewport, glm::mat4x4 modelMatrix = glm::mat4x4(1.0f));
		};
}