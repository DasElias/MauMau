#include "OpponentRenderer.h"
#include "CardStackPositions.h"
#include <shared/utils/MathUtils.h>
#include "CardStackRenderer.h"
#include "../renderingModel/DrawCardStackClamper.h"

namespace card {
	OpponentRenderer::OpponentRenderer(CardRenderer& cardRenderer, ProjectionMatrix& projectionMatrix, Viewport& viewport) :
			handCardsRenderer(cardRenderer),
			cardInterpolator(cardRenderer, projectionMatrix, viewport),
			projectionMatrix(projectionMatrix),
			viewport(viewport) {
	}
	void OpponentRenderer::renderOpponentIfHasValue_x(std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation) {
		auto& opp = opponentsOrNullInCwOrder[index];
		if(opp) {
			renderOpponent_x(opp->getCardStack(), handCardsPosition, handCardsRotation);
		}
	}
	void OpponentRenderer::renderOpponent_x(const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation) {
		handCardsRenderer.renderCardStackInX(handCardStack, handCardsPosition, handCardsRotation, projectionMatrix, viewport, FRONT_BACK_OPPONENT_CARDS_WIDTH);
	}
	void OpponentRenderer::renderOpponentIfHasValue_z(std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation) {
		auto& opp = opponentsOrNullInCwOrder[index];
		if(opp) {
			renderOpponent_z(opp->getCardStack(), handCardsPosition, handCardsRotation);
		}
	}
	void OpponentRenderer::renderOpponent_z(const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation) {
		handCardsRenderer.renderCardStackInZ(handCardStack, handCardsPosition, handCardsRotation, projectionMatrix, viewport, LEFT_RIGHT_OPPONENT_CARDS_WIDTH);
	}


	void OpponentRenderer::renderDrawedCardAnimationsOfOpponentIfHasValue(ProxyMauMauGame& game, std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation) {
		auto& opp = opponentsOrNullInCwOrder[index];
		if(opp) {
			renderDrawedCardAnimationsOfOpponent(game, opp->getCardStack(), handCardsPosition, handCardsRotation);
		}
	}
	void OpponentRenderer::renderDrawedCardAnimationsOfOpponent(ProxyMauMauGame& game, const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation) {
		const auto& drawStack = game.getDrawStack();
		std::size_t cardStackSize = DrawCardStackClamper::getClampedSize(drawStack);

		for(auto animation : handCardStack.getCardAnimations()) {
			float cardStackHeightAddition = CardStackRenderer::ADDITION_PER_CARD * cardStackSize;
			cardInterpolator.interpolateAndRender(animation,
												  DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition, 0),
												  DRAW_CARDS_ROTATION,
												  DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition + CardRenderer::HEIGHT / 2, -CardRenderer::HEIGHT),
												  DRAW_CARDS_ROTATION + glm::vec3(PI / 4, 0, 0),
												  handCardsPosition,
												  handCardsRotation,
												  0.33f, 0.66f);
		}
	}
	void OpponentRenderer::renderDrawedCardAnimationsOfOpponentIfHasValue(ProxyMauMauGame& game, std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, glm::vec3 middlePosition, glm::vec3 middleRotation) {
		auto& opp = opponentsOrNullInCwOrder[index];
		if(opp) {
			renderDrawedCardAnimationsOfOpponent(game, opp->getCardStack(), handCardsPosition, handCardsRotation, middlePosition, middleRotation);
		}
	}
	void OpponentRenderer::renderDrawedCardAnimationsOfOpponent(ProxyMauMauGame& game, const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, glm::vec3 middlePosition, glm::vec3 middleRotation) {
		const auto& drawStack = game.getDrawStack();
		std::size_t drawStackSize = DrawCardStackClamper::getClampedSize(drawStack);

		for(auto animation : handCardStack.getCardAnimations()) {
			float cardStackHeightAddition = CardStackRenderer::ADDITION_PER_CARD * drawStackSize;
			cardInterpolator.interpolateAndRender(animation,
												  DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition, 0),
												  DRAW_CARDS_ROTATION,
												  DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition + CardRenderer::HEIGHT / 2, -CardRenderer::HEIGHT),
												  DRAW_CARDS_ROTATION + glm::vec3(PI / 4, 0, 0),
												  middlePosition,
												  middleRotation,
												  handCardsPosition,
												  handCardsRotation,
												  0.33f, 0.33f);
		}
	}
}