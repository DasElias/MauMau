#include "LocalPlayerRenderer.h"
#include <shared/utils/TimeUtils.h>
#include <shared/utils/MathUtils.h>
#include <algorithm>
#include "CardStackPositions.h"
#include "DrawnCardRenderer.h"
#include "CardStackRenderer.h"
#include "../renderingModel/DrawCardStackClamper.h"
#include "../renderingModel/AnimationsToPlayStackFilter.h"

namespace card {
	float const LocalPlayerRenderer::HOVERED_CARD_Y_ADDITION = 0.1f;

	LocalPlayerRenderer::LocalPlayerRenderer(ProjectionMatrix& projectionMatrix, Viewport& viewport, CardRenderer& cardRenderer, CardStackMisalignmentGenerator& misalignmentGenerator) :
				projectionMatrix(projectionMatrix),
				viewport(viewport),
				cardRenderer(cardRenderer),
				handCardsRenderer(cardRenderer, projectionMatrix, viewport, misalignmentGenerator),
				animatorToPlayStack(cardRenderer, projectionMatrix, viewport, misalignmentGenerator),
				intersectionChecker(projectionMatrix, viewport),
				cardInterpolator(cardRenderer, projectionMatrix, viewport),
				unixTimeIntersectedCardIndexHasChanged(getMilliseconds()) {
	}
	void LocalPlayerRenderer::render(ProxyMauMauGame& game) {
		renderAnimations(game);
		renderHandCards(game);
	}
	void LocalPlayerRenderer::renderHandCards(ProxyMauMauGame& game) {
		auto& playStack = game.getPlayStack();
		auto[intersectedCardIndex, additionForIntersectedCard] = getIntersectedIndexAndAddition(game);

		// render hand cards and animation to play card stack
		HandCardStackPositionGenerator handCardPositionGenerator;
		std::vector<PositionedCard> positionedHandCards = handCardPositionGenerator.generateMatricies_cardStackX(getPositionedCardStackOfLocalPlayer(game), CardRenderer::CARD_DIMENSIONS, FRONT_BACK_OPPONENT_CARDS_WIDTH, intersectedCardIndex, additionForIntersectedCard);
		auto shouldDisableFunc = [&](Card c) {
			return shouldDisableCard(game, c);
		};
		handCardsRenderer.renderHandCardsOfLocalPlayer(positionedHandCards, game, shouldDisableFunc);

		// render animation to play card stack from temp card stack
		auto animationsToPlayCardStack = AnimationsToPlayStackFilterer::getAnimationsFromParticularHandCards(game, game.getLocalPlayer().getTempCardStack());
		for(auto& a : animationsToPlayCardStack) {
			animatorToPlayStack.animateFromLocalPlayerTemp(a, playStack);
		}
	}
	std::pair<int, float> LocalPlayerRenderer::getIntersectedIndexAndAddition(ProxyMauMauGame& game) {
		auto& gameData = game.getGameData();
		auto& handCards = game.getLocalPlayer().getCardStack();

		std::optional<int> intersectedCardIndexOrNone = checkIntersectionWithHandCards(game);
		int intersectedCardIndex = -1;
		if(intersectedCardIndexOrNone.has_value()) {
			Card intersectedCard = handCards.get(*intersectedCardIndexOrNone);
			if(!shouldDisableCard(game, intersectedCard) && gameData.isReadyToPerformLocalPlayerTurn()) {
				intersectedCardIndex = *intersectedCardIndexOrNone;
			}
		}

		if(intersectedCardIndex != intersectedCardIndexInLastFrame) {
			intersectedCardIndexInLastFrame = intersectedCardIndex;
			unixTimeIntersectedCardIndexHasChanged = getMilliseconds();
		}

		float x = getMilliseconds() - unixTimeIntersectedCardIndexHasChanged;
		float const x1 = 0;
		float const x2 = 100;
		float addition = interpolateLinear(x, x1, 0, x2, HOVERED_CARD_Y_ADDITION);
		addition = std::clamp<float>(addition, 0, HOVERED_CARD_Y_ADDITION);
		return std::make_pair(intersectedCardIndex, addition);
	}
	bool LocalPlayerRenderer::shouldDisableCard(ProxyMauMauGame& game, Card c) {
		auto& gameData = game.getGameData();
		auto& localPlayer = game.getLocalPlayer();

		if(! gameData.isReadyToPerformLocalPlayerTurn()) return false;

		if(localPlayer.isInSkipState()) return !gameData.canSkipPlayer(c);
		else if(gameData.isInDrawTwoState()) return gameData.getAmountsOfCardsToDrawForNextPlayer(c) == 0;
		else return false;
	}
	PositionedCardStack LocalPlayerRenderer::getPositionedCardStackOfLocalPlayer(ProxyMauMauGame& game) {
		auto& localPlayer = game.getLocalPlayer();
		return {localPlayer.getCardStack(), HAND_CARDS_LOCAL_POSITION, HAND_CARDS_LOCAL_ROTATION};
	}
	void LocalPlayerRenderer::renderAnimations(ProxyMauMauGame& game) {
		auto& localPlayer = game.getLocalPlayer();
		const auto& drawStack = game.getDrawStack();
		std::size_t drawStackSize = DrawCardStackClamper::getClampedSize(drawStack);

		// render cards to hand cards
		// please note that we iterate over the set from the end to the begin (we use a reverse-iterator),
		// since we want to render animations added later also to be rendered later
		// this is important when we have multiple CardAnimations from the draw card stack to the hand card stack,
		// for example when the player was forced to draw 2 cards
		auto localPlayerAnimations = localPlayer.getCardStack().getCardAnimations();
		for(auto it = localPlayerAnimations.rbegin(); it != localPlayerAnimations.rend(); ++it) {
			auto& animation = *it;

			if(animation.source.get().equalsId(drawStack)) {
				// render cards from draw card stack to hand cards
				float cardStackHeightAddition = CardStackRenderer::ADDITION_PER_CARD * drawStackSize;
				cardInterpolator.interpolateAndRender(animation,
										   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition, 0),
										   DRAW_CARDS_ROTATION,
										   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition + CardRenderer::HEIGHT / 4, CardRenderer::HEIGHT * 0.75f),
										   DRAW_CARDS_ROTATION - glm::vec3(PI / 4, 0, 0),
										   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition + CardRenderer::HEIGHT, CardRenderer::HEIGHT * 1.5f),
										   DRAW_CARDS_ROTATION - glm::vec3(PI / 2, 0, 0),
										   HAND_CARDS_LOCAL_POSITION,
										   HAND_CARDS_LOCAL_ROTATION);
			} else if(animation.source.get().equalsId(localPlayer.getTempCardStack())) {
				// render cards from temporary card stack to hand cards
				cardInterpolator.interpolateAndRender(animation,
										   DrawnCardRenderer::POSITION,
										   DrawnCardRenderer::ROTATION,
										   HAND_CARDS_LOCAL_POSITION,
										   HAND_CARDS_LOCAL_ROTATION
				);
			}
		}

		// render cards from draw card stack to temporary card stack
		for(auto& animation : localPlayer.getTempCardStack().getCardAnimations()) {
			cardInterpolator.interpolateAndRender(animation,
									   DRAW_CARDS_POSITION + glm::vec3(0, CardStackRenderer::ADDITION_PER_CARD * drawStackSize, 0),
									   DRAW_CARDS_ROTATION,
									   DRAW_CARDS_POSITION + glm::vec3(0, CardStackRenderer::ADDITION_PER_CARD * drawStackSize, CardRenderer::HEIGHT),
									   DRAW_CARDS_ROTATION - glm::vec3(PI / 4, 0, 0),
									   DrawnCardRenderer::POSITION,
									   DrawnCardRenderer::ROTATION
			);
		}
	}
	std::optional<int> LocalPlayerRenderer::checkIntersectionWithHandCards(ProxyMauMauGame& game) {
		auto pcs = getPositionedCardStackOfLocalPlayer(game);
		return intersectionChecker.getIndexOfIntersectedCardInX(pcs, CardRenderer::CARD_DIMENSIONS, FRONT_BACK_OPPONENT_CARDS_WIDTH);
	}
}