#pragma once
#include <shared/model/Card.h>
#include <vector>
#include <glm/glm.hpp>
#include "Renderer3D.h"
#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include "../renderingModel/CardTextures.h"
#include "../renderingModel/PositionedCard.h"

namespace card {
	#pragma pack(push, 1)
	struct InstancedCardData {
		glm::vec4 modelViewProjection_col1;
		glm::vec4 modelViewProjection_col2;
		glm::vec4 modelViewProjection_col3;
		glm::vec4 modelViewProjection_col4;
		std::uint32_t textureId;
	};
	#pragma pack(pop)

	class InstancedCardRenderingVao {
		public:
			InstancedCardRenderingVao(DataTextureVertexArrayObject wrappedVao);
			void update(const std::vector<InstancedCardData>& data);

		private:
			// 16 floats for MVP-matricies
			// 1 uint32_t for card texture id, but the last bit represents whether the card should be rendered in grey scale
			// this bit is 1, if the card should be rendered in grey scale
			static int const BYTES_PER_CARD = (16 * sizeof(float) + 1 * sizeof(std::uint32_t));

			DataTextureVertexArrayObject vao;
			std::uint32_t vboId;

			void addInstancedFloatAttribute(std::uint32_t index, std::uint32_t offsetInBytes, std::uint32_t amountOfFloats);
			void addInstacedIntegerAttribute(std::uint32_t index, std::uint32_t offsetInBytes, std::uint32_t amountOfInteger);
	};

	class CardRendererShader : public VertexFragmentShaderProgram {
		public:
			CardRendererShader();
	};
	
	class CardRenderer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static float const WIDTH;
			static float const HEIGHT;

		private:
			static std::vector<float> const VERTEX_DATA;
			static std::vector<float> const TEXTURE_DATA;
			static uint32_t const GREY_SCALE_BIT_MASK = 1u << 31;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			Renderer3D& renderer3d;
			DataTextureVertexArrayObject singleCardVao;
			InstancedCardRenderingVao instancedVbo;
			CardRendererShader shader;
			CardTextures cardTextures;

			std::vector<InstancedCardData> cardsToRenderInNextPass;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CardRenderer(Renderer3D& renderer3d, CardTextures cardTextures);
			CardRenderer(const CardRenderer&) = delete;
			~CardRenderer();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void renderInNextPass(const PositionedCard& card, ProjectionMatrix& projectionMatrix, Viewport& viewport, bool shouldRenderInGreyScale = false);
			void renderInNextPass(const std::vector<PositionedCard>& cards, ProjectionMatrix& projectionMatrix, Viewport& viewport, std::vector<bool> shouldRenderInGreyScaleVector = {});
			void flush(bool renderWithHighAnisotropicFiltering = false);

		private:
			std::uint32_t convertCardNumberToTextureId(std::uint32_t cardNumber);
	};
}