#pragma once
#include <array>
#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include <egui/model\positioning\PositionableElement.h>
#include "../renderingModel/VertexArrayObject.h"

namespace card {
	class Renderer2D {
		class Renderer2DShader : public VertexFragmentShaderProgram {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				int location_transformationMatrix;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				Renderer2DShader(bool discardTransparentFragments);

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void loadTransformationMatrix(glm::mat4x4 transformationMatrix);
		};

		class Renderer2DVertexArrayObject : public VertexArrayObject {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				std::uint32_t vertexVbo;
				std::uint32_t textureVbo;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				Renderer2DVertexArrayObject(const std::vector<float>& data);

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void loadTexData(const std::array<float, 8>& texData);
		};



		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::vector<float> const POSITIONS;
			static std::vector<float> const TEXTURE_COORDS;


		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			Renderer2DShader shader;
			Renderer2DVertexArrayObject quad;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Renderer2D(bool discardTransparentFragments = false);
			Renderer2D(const Renderer2D&) = delete;
			Renderer2D& operator=(const Renderer2D&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			glm::mat4x4 computeTransformationMatrix(const std::shared_ptr<egui::PositionableElement>& elem) const;

		public:
			void render(std::shared_ptr<egui::PositionableElement> elem, bool hasTransparency = false,
					glm::vec2 topLeftTexture = {0, 0}, glm::vec2 topRightTexture = {1, 0}, 
					glm::vec2 bottomRightTexture = {1, 1}, glm::vec2 bottomLeftTexture = {0, 1});
	};
}