#include "CardStackRenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <utility>

namespace card {
	float const CardStackRenderer::ADDITION_PER_CARD = 0.01f;

	CardStackRenderer::CardStackRenderer(CardRenderer& cardRendererImpl) :
			cardRendererImpl(cardRendererImpl) {
	}
	void CardStackRenderer::renderCardStack(const CardAnimator& cardStack, glm::vec3 position, glm::vec3 rotation, ProjectionMatrix& projectionMatrix, Viewport& viewport) {
		std::vector<PositionedCard> cards;
		for(auto c : cardStack) {	
			cards.push_back({c, position, rotation});
			position.y += ADDITION_PER_CARD;
		}

		cardRendererImpl.renderInNextPass(cards, projectionMatrix, viewport);
	}
}