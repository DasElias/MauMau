#include "HandCardStackRenderer.h"
#include "HandCardStackRenderer.h"
#include "HandCardStackRenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

namespace card {
	float const HandCardStackRenderer::CARD_OVERLAPPING_FACTOR = 0.5f;

	HandCardStackRenderer::HandCardStackRenderer(CardRenderer& cardRendererImpl, ProjectionMatrix& projectionMatrix, Viewport& viewport) :
			cardRendererImpl(cardRendererImpl),
			projectionMatrix(projectionMatrix),
			viewport(viewport) {
	}
	void HandCardStackRenderer::renderCardStackInX(PositionedCardStack positionedCardStack, float maxWidth, int selectedCardIndex, float selectedCardAddition, std::vector<bool> shouldRenderInGreyScale) {
		auto cards = positionGenerator.generateMatricies_cardStackX(positionedCardStack, CardRenderer::CARD_DIMENSIONS, maxWidth, selectedCardIndex, selectedCardAddition);
		cardRendererImpl.renderInNextPass(cards, projectionMatrix, viewport, shouldRenderInGreyScale);
	}
	void HandCardStackRenderer::renderCardStackInZ(PositionedCardStack positionedCardStack, float maxWidth, int selectedCardIndex, float selectedCardAddition, std::vector<bool> shouldRenderInGreyScale) {
		auto cards = positionGenerator.generateMatricies_cardStackZ(positionedCardStack, CardRenderer::CARD_DIMENSIONS, maxWidth, selectedCardIndex, selectedCardAddition);
		cardRendererImpl.renderInNextPass(cards, projectionMatrix, viewport, shouldRenderInGreyScale);
	}
}