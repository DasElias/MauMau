#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DrawCardStackRenderer.h"
#include "CardRenderer.h"
#include "../renderingModel/SimpleTextureFactory.h"
#include <glm/gtc/matrix_transform.hpp>
#include "CardStackRenderer.h"
#include "../renderingModel/DrawCardStackClamper.h"
#include <iostream>
#include <shared/utils/VectorUtils.h>
#include <shared/utils/MathUtils.h>
#include <res/ingame/cardStackFront.png.h>

namespace card {
	float const DrawCardStackRenderer::BORDER_RADIUS_WIDTH = (6.0f / 350) * CardRenderer::WIDTH;
	float const DrawCardStackRenderer::BORDER_RADIUS_HEIGHT = (6.0f / 506) * CardRenderer::HEIGHT;

	static float const START_X = -CardRenderer::WIDTH / 2;
	static float const END_X = CardRenderer::WIDTH / 2;
	static float const START_Y = -CardRenderer::HEIGHT / 2;
	static float const END_Y = CardRenderer::HEIGHT / 2;
	static float const START_Z = -1;
	static float const END_Z = 0;
	DrawCardStackRenderer::DrawCardStackRenderer(CardRenderer& cardRenderer, Renderer3D& renderer3D, ProjectionMatrix& pm, Viewport& vp) :
			cardRenderer(cardRenderer),
			renderer3D(renderer3D),
			projectionMatrix(pm),
			viewport(vp),
			borderVao(VertexArrayObject::RenderMode::TRIANGLES, 3, genBorderData(), genColorData(10)),
			texture(
				SimpleTextureFactory()
				.setWrapS(TextureWrap::REPEAT)
				.setWrapT(TextureWrap::REPEAT)
				.setMinFilter(TextureMinFilter::LINEAR_MIPMAP_NEAREST)
				.setMagFilter(TextureMagFilter::LINEAR)
				.loadFromMemory(tex_cardStackFront, tex_cardStackFront_size)
			) {
	}
	static void appendFace(std::vector<float>& data, glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight) {
		data += {bottomRight.x, bottomRight.y, bottomRight.z};
		data += {bottomLeft.x, bottomLeft.y, bottomLeft.z};
		data += {topLeft.x, topLeft.y, topLeft.z};

		data += {bottomRight.x, bottomRight.y, bottomRight.z};
		data += {topLeft.x, topLeft.y, topLeft.z};
		data += {topRight.x, topRight.y, topRight.z};
	}
	std::vector<float> DrawCardStackRenderer::genBorderData() {
		std::vector<float> data;
		// front (top)
		appendFace(data, {START_X + BORDER_RADIUS_WIDTH, END_Y, END_Z}, {END_X - BORDER_RADIUS_WIDTH, END_Y, END_Z}, {START_X + BORDER_RADIUS_WIDTH, END_Y, START_Z}, {END_X - BORDER_RADIUS_WIDTH, END_Y, START_Z});
		
		// right (left)
		appendFace(data, {END_X, END_Y - BORDER_RADIUS_HEIGHT, END_Z}, {END_X, START_Y + BORDER_RADIUS_HEIGHT, END_Z}, {END_X, END_Y - BORDER_RADIUS_HEIGHT, START_Z}, {END_X, START_Y + BORDER_RADIUS_HEIGHT, START_Z});
		
		// diagonale between front/right
		appendFace(data, {END_X - BORDER_RADIUS_WIDTH, END_Y, START_Z}, {END_X - BORDER_RADIUS_WIDTH, END_Y, END_Z}, {END_X, END_Y - BORDER_RADIUS_HEIGHT, START_Z}, {END_X, END_Y - BORDER_RADIUS_HEIGHT, END_Z});

		// diagonale between front/left
		appendFace(data, {START_X, END_Y - BORDER_RADIUS_HEIGHT, START_Z}, {START_X, END_Y - BORDER_RADIUS_HEIGHT, END_Z}, {START_X + BORDER_RADIUS_WIDTH, END_Y, START_Z}, {START_X + BORDER_RADIUS_WIDTH, END_Y, END_Z});


		return data;
	}
	std::vector<float> DrawCardStackRenderer::genColorData(float amountOfCards) {
		float amountOfCardsFloat = amountOfCards;
		return {
			1, amountOfCardsFloat,
			0, amountOfCardsFloat,
			0, 0,
			1, amountOfCardsFloat,
			0, 0,
			1, 0,

			1, amountOfCardsFloat,
			0, amountOfCardsFloat,
			0, 0,
			1, amountOfCardsFloat,
			0, 0,
			1, 0,

			1, amountOfCardsFloat,
			0, amountOfCardsFloat,
			0, 0,
			1, amountOfCardsFloat,
			0, 0,
			1, 0,

			1, amountOfCardsFloat,
			0, amountOfCardsFloat,
			0, 0,
			1, amountOfCardsFloat,
			0, 0,
			1, 0,
		};
	}
	void DrawCardStackRenderer::render(PositionedCardStack positionedCardStack, bool shouldRenderDisabled) {
		int cardStackSize = DrawCardStackClamper::getClampedSize(positionedCardStack.getCardAnimator());

		PositionedCard p = {Card::NULLCARD, positionedCardStack.getCenterPosition(), positionedCardStack.getRotation()};
		glm::mat4 modelMatrix = p.getModelMatrix();
		float height = (cardStackSize - 1) * CardStackRenderer::ADDITION_PER_CARD;
		modelMatrix = glm::scale(modelMatrix, {1, 1, height});

		borderVao.updateTexture(genColorData(cardStackSize / 64.0f));
		texture.bind();
		renderer3D.render(borderVao, projectionMatrix, viewport, modelMatrix);

		p.changePosition({0, height, 0});
		cardRenderer.renderInNextPass(p, projectionMatrix, viewport, shouldRenderDisabled);
	}
}