#pragma once
#include <array>
#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include <egui/model\positioning\PositionableElement.h>
#include "../renderingModel/ProjectionMatrix.h"
#include "../renderingModel/Viewport.h"
#include "../renderingModel/DataColorVertexArrayObject.h"

namespace card {
	class ColorRenderer3D {
		class ColorRenderer3DShader : public VertexFragmentShaderProgram {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				int location_projectionViewMatrix;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				ColorRenderer3DShader();

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
			ColorRenderer3DShader shader;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ColorRenderer3D() = default;
			ColorRenderer3D(const ColorRenderer3D&) = delete;
			ColorRenderer3D& operator=(const ColorRenderer3D&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void render(const DataColorVertexArrayObject& vertexArrayObject, const ProjectionMatrix& projectionMatrix, const Viewport& viewport, glm::mat4x4 modelMatrix = glm::mat4x4(1.0f));
	};
}