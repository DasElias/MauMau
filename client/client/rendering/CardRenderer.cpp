#include "CardRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../renderingModel/BufferOffsetMacro.h"

#include "shaderPrograms/shaders/cards_vertex.glsl"
#include "shaderPrograms/shaders/cards_fragment.glsl"

#include <shared/utils/MathUtils.h>

namespace card {
	float const CardRenderer::WIDTH = 0.5f;
	float const CardRenderer::HEIGHT = WIDTH * 8/5;

	float const X = CardRenderer::WIDTH / 2;
	float const Z = 0;

	std::vector<float> const CardRenderer::VERTEX_DATA = {
		-X, HEIGHT / 2, Z,
		-X, -HEIGHT / 2, Z,
		X, -HEIGHT / 2, Z,
		-X, HEIGHT / 2, Z,
		X, -HEIGHT / 2, Z,
		X, HEIGHT / 2, Z,
	};
	std::vector<float> const CardRenderer::TEXTURE_DATA = {
		0, 0,
		0, 1,
		1, 1,
		0, 0,
		1, 1,
		1, 0,
	};

	InstancedCardRenderingVao::InstancedCardRenderingVao(DataTextureVertexArrayObject wrappedVao) :
			vao(wrappedVao) {
		glBindVertexArray(vao.getVertexArrayObjectId());
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		// * 2, because every card has a back side
		glBufferData(GL_ARRAY_BUFFER, Card::MAX_CARDS * BYTES_PER_CARD * 2, nullptr, GL_DYNAMIC_DRAW);


		// 4 Spalten der Matrix
		addInstancedFloatAttribute(2, 0 * sizeof(float), 4);
		addInstancedFloatAttribute(3, 4 * sizeof(float), 4);
		addInstancedFloatAttribute(4, 8 * sizeof(float), 4);
		addInstancedFloatAttribute(5, 12 * sizeof(float), 4);
		addInstacedIntegerAttribute(6, 16 * sizeof(float), 1);

	}
	void InstancedCardRenderingVao::addInstancedFloatAttribute(std::uint32_t index, std::uint32_t offsetInBytes, std::uint32_t amountOfFloats) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, amountOfFloats, GL_FLOAT, false, BYTES_PER_CARD, BUFFER_OFFSET(offsetInBytes));

		// per-instance-vbo
		glVertexAttribDivisor(index, 1);
	}
	void InstancedCardRenderingVao::addInstacedIntegerAttribute(std::uint32_t index, std::uint32_t offsetInBytes, std::uint32_t amountOfInteger) {
		glEnableVertexAttribArray(index);
		glVertexAttribIPointer(index, amountOfInteger, GL_INT, BYTES_PER_CARD, BUFFER_OFFSET(offsetInBytes));

		glVertexAttribDivisor(index, 1);
	}
	void InstancedCardRenderingVao::update(const std::vector<InstancedCardData>& data) {
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(InstancedCardData), (data.empty()) ? nullptr : &data[0], GL_DYNAMIC_DRAW);
		//TODO check if it is acceptable to remove glBufferSubData here
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	CardRendererShader::CardRendererShader() :
		VertexFragmentShaderProgram(
			cards_vertex,
			cards_fragment,
			[this]() {
				this->createShaderVariable(0, "position");
				this->createShaderVariable(1, "texCoords");
				this->createShaderVariable(2, "modelViewProjectionMatrix");
				this->createShaderVariable(6, "textureId");
			}) {
	}
	CardRenderer::CardRenderer(Renderer3D& renderer3d, CardTextures cardTextures) :
			renderer3d(renderer3d),
			singleCardVao(VertexArrayObject::TRIANGLES, 3, VERTEX_DATA, TEXTURE_DATA),
			instancedVbo(singleCardVao),
			cardTextures(cardTextures) {
	}
	CardRenderer::~CardRenderer() {
		singleCardVao.cleanUp();
	}
	void CardRenderer::renderInNextPass(const PositionedCard& card, ProjectionMatrix& projectionMatrix, Viewport& viewport) {
		glm::mat4 frontMVPMatrix = projectionMatrix.getProjectionMatrix() * viewport.getViewMatrix() * card.getModelMatrix();
		this->cardsToRenderInNextPass.push_back({frontMVPMatrix[0], frontMVPMatrix[1], frontMVPMatrix[2], frontMVPMatrix[3], card.getCardNumber()});

		// draw back side of the card
		PositionedCard backside = card;
		backside.changeRotation({0, PI, 0});
		backside.setCard({0});
		glm::mat4 backMVPMatrix = projectionMatrix.getProjectionMatrix() * viewport.getViewMatrix() * backside.getModelMatrix();
		this->cardsToRenderInNextPass.push_back({backMVPMatrix[0], backMVPMatrix[1], backMVPMatrix[2], backMVPMatrix[3], backside.getCardNumber()});
	}
	void CardRenderer::renderInNextPass(const std::vector<PositionedCard>& cards, ProjectionMatrix& projectionMatrix, Viewport& viewport) {
		for(auto& c : cards) {
			this->renderInNextPass(c, projectionMatrix, viewport);
		}
	}
	void CardRenderer::flush(bool renderWithHighAnisotropy) {
		if(this->cardsToRenderInNextPass.empty()) return;

		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);

		instancedVbo.update(this->cardsToRenderInNextPass);

		shader.startProgram();
		glBindVertexArray(singleCardVao.getVertexArrayObjectId());

		glActiveTexture(GL_TEXTURE0);
		if(renderWithHighAnisotropy) cardTextures.bindWithHighAnisotropy();
		else cardTextures.bindDefault();
		glDrawArraysInstanced(singleCardVao.getRenderMode(), 0, singleCardVao.getIndiciesCount(), GLsizei(cardsToRenderInNextPass.size()));
		glBindVertexArray(0);
		cardTextures.unbind();
		shader.stopProgram();

		this->cardsToRenderInNextPass.clear();
	}
	/*void CardRenderer::render(const std::vector<PositionedCard>& positionedCards, ProjectionMatrix& projectionMatrix, Viewport& viewport) {
		std::vector<InstancedCardData> updateData;
		for(auto& card : positionedCards) {
			glm::mat4 frontMVPMatrix = projectionMatrix.getProjectionMatrix() * viewport.getViewMatrix() * card.getModelMatrix();
			updateData.push_back({frontMVPMatrix[0], frontMVPMatrix[1], frontMVPMatrix[2], frontMVPMatrix[3], card.getCardNumber()});

			// draw back side of the card
			PositionedCard backside = card;
			backside.changeRotation({0, PI, 0});
			backside.setCard({0});
			glm::mat4 backMVPMatrix = projectionMatrix.getProjectionMatrix() * viewport.getViewMatrix() * backside.getModelMatrix();
			updateData.push_back({backMVPMatrix[0], backMVPMatrix[1], backMVPMatrix[2], backMVPMatrix[3], backside.getCardNumber()});
		}
		instancedVbo.update(updateData);

		shader.startProgram();
		glBindVertexArray(singleCardVao.getVertexArrayObjectId());
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, cardTextures.getTexId());
		glDrawArraysInstanced(singleCardVao.getRenderMode(), 0, singleCardVao.getIndiciesCount(), updateData.size());
		glBindVertexArray(0);
		shader.stopProgram();
	}*/
}