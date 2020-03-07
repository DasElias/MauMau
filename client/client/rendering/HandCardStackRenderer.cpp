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

	HandCardStackRenderer::HandCardStackRenderer(CardRenderer& cardRendererImpl) :
			cardRendererImpl(cardRendererImpl) {
	}
	void HandCardStackRenderer::renderCardStackInX(const CardAnimator& cardStack, glm::vec3 centerPosition, glm::vec3 rotation, ProjectionMatrix& projectionMatrix, Viewport& viewport, float maxWidth, int selectedCardIndex, float selectedCardAddition) {
		auto cards = positionGenerator.generateMatricies_cardStackX(cardStack, centerPosition, rotation, maxWidth, CardRenderer::WIDTH, selectedCardIndex, selectedCardAddition);
		cardRendererImpl.renderInNextPass(cards, projectionMatrix, viewport);
	}
	void HandCardStackRenderer::renderCardStackInZ(const CardAnimator& cardStack, glm::vec3 centerPosition, glm::vec3 rotation, ProjectionMatrix& projectionMatrix, Viewport& viewport, float maxWidth, int selectedCardIndex, float selectedCardAddition) {
		auto cards = positionGenerator.generateMatricies_cardStackZ(cardStack, centerPosition, rotation, maxWidth, CardRenderer::WIDTH, selectedCardIndex, selectedCardAddition);
		cardRendererImpl.renderInNextPass(cards, projectionMatrix, viewport);
	}
}