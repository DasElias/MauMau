#include "CardStackRenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <utility>

namespace card {
	float const CardStackRenderer::ADDITION_PER_CARD = 0.0075f;

	CardStackRenderer::CardStackRenderer(CardRenderer& cardRendererImpl, CardStackMisalignmentGenerator& misalignmentGenerator, ProjectionMatrix& projectionMatrix, Viewport& viewport) :
			cardRendererImpl(cardRendererImpl),
			misalignmentGenerator(misalignmentGenerator),
			projectionMatrix(projectionMatrix),
			viewport(viewport) {
	}
	void CardStackRenderer::renderCardStack(PositionedCardStack positionedCardStack, bool shouldDisable) {
		const CardAnimator& cardStack = positionedCardStack.getCardAnimator();
		glm::vec3 position = positionedCardStack.getCenterPosition();
		glm::vec3 rotation = positionedCardStack.getRotation();
		
		std::vector<PositionedCard> cards;
		for(int i = 0; i < cardStack.getSize(); i++) {	
			auto& c = cardStack.get(i);
			cards.push_back({c, position, rotation});
			position.y += ADDITION_PER_CARD;
		}

		if(shouldDisable) {
			std::vector<bool> disabledCardVector(cardStack.getSize(), true);
			cardRendererImpl.renderInNextPass(cards, projectionMatrix, viewport, disabledCardVector);
		} else {
			cardRendererImpl.renderInNextPass(cards, projectionMatrix, viewport);
		}
	}

	void CardStackRenderer::renderCardStackWithMisalignment(PositionedCardStack positionedCardStack, bool shouldDisable) {
		const CardAnimator& cardStack = positionedCardStack.getCardAnimator();
		glm::vec3 position = positionedCardStack.getCenterPosition();
		glm::vec3 rotation = positionedCardStack.getRotation();

		std::vector<PositionedCard> cards;
		for(std::size_t i = 0; i < cardStack.getSize(); i++) {
			auto c = cardStack.get(i);

			cards.push_back({c, position, rotation + misalignmentGenerator.computeRotationMisalignment(i)});
			position.y += ADDITION_PER_CARD;
		}

		if(shouldDisable) {
			std::vector<bool> disabledCardVector(cardStack.getSize(), true);
			cardRendererImpl.renderInNextPass(cards, projectionMatrix, viewport, disabledCardVector);
		} else {
			cardRendererImpl.renderInNextPass(cards, projectionMatrix, viewport);
		}
	}
}