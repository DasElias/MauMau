#include "OpponentRenderer.h"
#include "CardStackPositions.h"
#include <shared/utils/MathUtils.h>
#include "CardStackRenderer.h"
#include "../renderingModel/DrawCardStackClamper.h"

namespace card {
	OpponentRenderer::OpponentRenderer(CardRenderer& cardRenderer, ProjectionMatrix& projectionMatrix, Viewport& viewport, CardStackMisalignmentGenerator& playStackMisalignmentGenerator) :
			handCardsRenderer(cardRenderer, projectionMatrix, viewport),
			cardInterpolator(cardRenderer, projectionMatrix, viewport),
			handCardsWithInterpolationRenderer(cardRenderer, projectionMatrix, viewport, playStackMisalignmentGenerator),
			projectionMatrix(projectionMatrix),
			viewport(viewport) {
	}
	void OpponentRenderer::renderHandCardsOfVisAVisOpponent(ProxyMauMauGame& game, ProxyPlayer& player) {
		PositionedCardStack handCards(player.getCardStack(), HAND_CARDS_OPPONENT_VISAVIS_POSITION, HAND_CARDS_OPPONENT_VISAVIS_ROTATION);
		auto positionedCards = handCardStackPositionGenerator.generateMatricies_cardStackX(handCards, CardRenderer::CARD_DIMENSIONS, FRONT_BACK_OPPONENT_CARDS_WIDTH);
		handCardsWithInterpolationRenderer.renderHandCardsOfVisavisOpponent(positionedCards, player.getCardStack(), game);		
	}
	void OpponentRenderer::renderHandCardsOfLeftOpponent(ProxyMauMauGame& game, ProxyPlayer& player) {
		PositionedCardStack handCards(player.getCardStack(), HAND_CARDS_OPPONENT_LEFT_POSITION, HAND_CARDS_OPPONENT_LEFT_ROTATION);
		auto positionedCards = handCardStackPositionGenerator.generateMatricies_cardStackZ(handCards, CardRenderer::CARD_DIMENSIONS, LEFT_RIGHT_OPPONENT_CARDS_WIDTH);
		handCardsWithInterpolationRenderer.renderHandCardsOfLeftOpponent(positionedCards, player.getCardStack(), game);
	}
	void OpponentRenderer::renderHandCardsOfRightOpponent(ProxyMauMauGame& game, ProxyPlayer& player) {
		PositionedCardStack handCards(player.getCardStack(), HAND_CARDS_OPPONENT_RIGHT_POSITION, HAND_CARDS_OPPONENT_RIGHT_ROTATION);
		auto positionedCards = handCardStackPositionGenerator.generateMatricies_cardStackZ(handCards, CardRenderer::CARD_DIMENSIONS, LEFT_RIGHT_OPPONENT_CARDS_WIDTH);
		handCardsWithInterpolationRenderer.renderHandCardsOfRightOpponent(positionedCards, player.getCardStack(), game);
	}
	void OpponentRenderer::renderAnimationsFromDrawStackOfVisAVisOpponent(ProxyMauMauGame& game, ProxyPlayer& player) {
		renderAnimationsFromDrawStack(game, player.getCardStack(), HAND_CARDS_OPPONENT_VISAVIS_POSITION, HAND_CARDS_OPPONENT_VISAVIS_ROTATION);
	}
	void OpponentRenderer::renderAnimationsFromDrawStackOfLeftOpponent(ProxyMauMauGame& game, ProxyPlayer& player) {
		renderAnimationsFromDrawStack(game, player.getCardStack(), HAND_CARDS_OPPONENT_LEFT_POSITION, HAND_CARDS_OPPONENT_LEFT_ROTATION);
	}
	void OpponentRenderer::renderAnimationsFromDrawStackOfRightOpponent(ProxyMauMauGame& game, ProxyPlayer& player) {
		renderAnimationsFromDrawStack(game, player.getCardStack(), HAND_CARDS_OPPONENT_RIGHT_POSITION, HAND_CARDS_OPPONENT_RIGHT_ROTATION, PLAY_CARDS_POSITION + glm::vec3(0, CardRenderer::HEIGHT / 2, -CardRenderer::HEIGHT), {PI, 0, 0});
	}
	void OpponentRenderer::renderAnimationsFromDrawStack(ProxyMauMauGame& game, const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation) {
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
	void OpponentRenderer::renderAnimationsFromDrawStack(ProxyMauMauGame& game, const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, glm::vec3 middlePosition, glm::vec3 middleRotation) {
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