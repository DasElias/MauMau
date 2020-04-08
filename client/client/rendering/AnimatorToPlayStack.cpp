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

		glm::vec3 startPosition = glm::vec3(0, LocalPlayerRenderer::HOVERED_CARD_Y_ADDITION, 0) + handCardPositionGenerator.getPositionOfCard_cardStackX(animation.indexInSourceStack, sourceStack.getSize() + 1, HAND_CARDS_LOCAL_POSITION, FRONT_BACK_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH);
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
	void AnimatorToPlayStack::animateFromLeftOpponent(const CardAnimator& cardAnimator, const CardAnimator& playStack) {
	}
	void AnimatorToPlayStack::animateFromRightOpponent(const CardAnimator& cardAnimator, const CardAnimator& playStack) {
	}
	void AnimatorToPlayStack::animateFromVisavisOpponent(const CardAnimator& cardAnimator, const CardAnimator& playStack) {
	}
	std::pair<glm::vec3, glm::vec3> AnimatorToPlayStack::getEndPositionAndRotation(const CardAnimator& playStack) {
		glm::vec3 positionEnd = PLAY_CARDS_POSITION + glm::vec3(0, playStack.getSize() * CardStackRenderer::ADDITION_PER_CARD, 0);
		glm::vec3 rotationEnd = PLAY_CARDS_ROTATION + misalignmentGenerator.computeRotationMisalignment(playStack.getSize());
		return std::make_pair(positionEnd, rotationEnd);
	}
}