#include "AnimatorToPlayStack.h"
#include "AnimatorToPlayStack.h"
#include "CardStackPositions.h"
#include "CardStackRenderer.h"
#include "LocalPlayerRenderer.h"
#include "DrawnCardRenderer.h"

namespace card {
	AnimatorToPlayStack::AnimatorToPlayStack(CardRenderer& cardRenderer, ProjectionMatrix& pm, Viewport& vp, CardStackMisalignmentGenerator& misalignmentGenerator) :
			cardInterpolator(cardRenderer, pm, vp),
			misalignmentGenerator(misalignmentGenerator) {
	}
	void AnimatorToPlayStack::animateFromLocalPlayerHandCards(const CardAnimation& animation, const CardAnimator& playStack) {
		auto& sourceStack = animation.source.get();
		auto[positionEnd, rotationEnd] = getEndPositionAndRotation(playStack);

		glm::vec3 startPosition = glm::vec3(0, LocalPlayerRenderer::HOVERED_CARD_Y_ADDITION, 0) + handCardStackPositionGenerator.getPositionOfCard_cardStackX(animation.indexInSourceStack, sourceStack.getSize() + 1, HAND_CARDS_LOCAL_POSITION, FRONT_BACK_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
		cardInterpolator.interpolateAndRender(animation,
											  startPosition, HAND_CARDS_LOCAL_ROTATION,
											  startPosition + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), HAND_CARDS_LOCAL_ROTATION,
											  positionEnd, rotationEnd
		);
	}
	void AnimatorToPlayStack::animateFromLocalPlayerTemp(const CardAnimation& animation, const CardAnimator& playStack) {
		auto& sourceStack = animation.source.get();
		auto [positionEnd, rotationEnd] = getEndPositionAndRotation(playStack);

		cardInterpolator.interpolateAndRender(animation,
											  DrawnCardRenderer::POSITION, HAND_CARDS_LOCAL_ROTATION,
											  positionEnd, rotationEnd
		);
	}
	void AnimatorToPlayStack::animateFromLeftOpponent(const CardAnimation& animation, const CardAnimator& playStack) {
		auto& sourceStack = animation.source.get();
		auto [positionEnd, rotationEnd] = getEndPositionAndRotation(playStack);

		glm::vec3 startPosition = handCardStackPositionGenerator.getPositionOfCard_cardStackZ(animation.indexInSourceStack, sourceStack.getSize() + 1, HAND_CARDS_OPPONENT_LEFT_POSITION, LEFT_RIGHT_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
		cardInterpolator.interpolateAndRender(animation,
											  startPosition, glm::vec3(0, -PI / 2, 0),
											  startPosition + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), glm::vec3(0, -PI / 2, 0),
											  positionEnd, {rotationEnd.x, -rotationEnd.y, rotationEnd.z}
		);
	}
	void AnimatorToPlayStack::animateFromRightOpponent(const CardAnimation& animation, const CardAnimator& playStack) {
		auto& sourceStack = animation.source.get();
		auto [positionEnd, rotationEnd] = getEndPositionAndRotation(playStack);

		glm::vec3 startPosition = handCardStackPositionGenerator.getPositionOfCard_cardStackZ(animation.indexInSourceStack, sourceStack.getSize() + 1, HAND_CARDS_OPPONENT_RIGHT_POSITION, LEFT_RIGHT_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
		cardInterpolator.interpolateAndRender(animation,
											  HAND_CARDS_OPPONENT_RIGHT_POSITION, glm::vec3(0, PI / 2, 0),
											  HAND_CARDS_OPPONENT_RIGHT_POSITION + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), glm::vec3(0, PI / 2, 0),
											  positionEnd, rotationEnd
		);
	}
	void AnimatorToPlayStack::animateFromVisavisOpponent(const CardAnimation& animation, const CardAnimator& playStack) {
		auto& sourceStack = animation.source.get();
		auto [positionEnd, rotationEnd] = getEndPositionAndRotation(playStack);

		glm::vec3 startPosition = handCardStackPositionGenerator.getPositionOfCard_cardStackX(animation.indexInSourceStack, sourceStack.getSize() + 1, HAND_CARDS_OPPONENT_VISAVIS_POSITION, FRONT_BACK_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
		cardInterpolator.interpolateAndRender(animation,
											  startPosition, {-PI, 0, 0},
											  startPosition + glm::vec3(0, CardRenderer::HEIGHT / 2, 0), {-PI, 0, 0},
											  positionEnd, rotationEnd
		);
	}
	std::pair<glm::vec3, glm::vec3> AnimatorToPlayStack::getEndPositionAndRotation(const CardAnimator& playStack) {
		glm::vec3 positionEnd = PLAY_CARDS_POSITION + glm::vec3(0, playStack.getSize() * CardStackRenderer::ADDITION_PER_CARD, 0);
		glm::vec3 rotationEnd = PLAY_CARDS_ROTATION + misalignmentGenerator.computeRotationMisalignment(playStack.getSize());
		return std::make_pair(positionEnd, rotationEnd);
	}
}