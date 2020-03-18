#include "LocalPlayerRenderer.h"
#include "LocalPlayerRenderer.h"
#include "LocalPlayerRenderer.h"
#include <shared/utils/TimeUtils.h>
#include <shared/utils/MathUtils.h>
#include <algorithm>
#include "CardStackPositions.h"
#include "DrawnCardRenderer.h"
#include "CardStackRenderer.h"

namespace card {
	float const LocalPlayerRenderer::HOVERED_CARD_Y_ADDITION = 0.1f;

	LocalPlayerRenderer::LocalPlayerRenderer(ProjectionMatrix& projectionMatrix, Viewport& viewport, CardRenderer& cardRenderer) :
				projectionMatrix(projectionMatrix),
				viewport(viewport),
				intersectionChecker(projectionMatrix, viewport),
				handCardStackRenderer(cardRenderer),
				cardInterpolator(cardRenderer, projectionMatrix, viewport),
				unixTimeIntersectedCardIndexHasChanged(getMilliseconds()) {
	}
	void LocalPlayerRenderer::render(ProxyMauMauGame& game) {
		renderHandCards(game);
		renderAnimations(game);
	}
	void LocalPlayerRenderer::renderHandCards(ProxyMauMauGame& game) {
		auto& accessorFromClient = game.getAccessorFromClient();
		auto& gameData = game.getGameData();
		auto& localPlayer = game.getLocalPlayer();
		auto& handCards = localPlayer->getCardStack();

		std::optional<int> intersectedCardIndexOrNone = checkIntersectionWithHandCards(game);
		int intersectedCardIndex = -1;
		if(intersectedCardIndexOrNone.has_value()) {
			Card intersectedCard = handCards.get(*intersectedCardIndexOrNone);
			if(! shouldDisableCard(game, intersectedCard) && gameData.isReadyToPerformLocalPlayerTurn()) {
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

		std::vector<bool> shouldRenderInGreyScale(handCards.getSize());
		for(int i = 0; i < shouldRenderInGreyScale.size(); i++) {
			Card& c = handCards.get(i);
			shouldRenderInGreyScale[i] = shouldDisableCard(game, c);
		}
		handCardStackRenderer.renderCardStackInX(localPlayer->getCardStack(), HAND_CARDS_LOCAL_POSITION, HAND_CARDS_LOCAL_ROTATION, projectionMatrix, viewport, FRONT_BACK_OPPONENT_CARDS_WIDTH, intersectedCardIndex, addition, shouldRenderInGreyScale);
	}
	bool LocalPlayerRenderer::shouldDisableCard(ProxyMauMauGame& game, Card c) {
		auto& gameData = game.getGameData();
		auto& localPlayer = game.getLocalPlayer();
		return localPlayer->isInSkipState() && !gameData.canSkipPlayer(c);
	}
	void LocalPlayerRenderer::renderAnimations(ProxyMauMauGame& game) {
		auto& localPlayer = game.getLocalPlayer();
		const auto& drawStack = game.getDrawStack();

		// render cards to hand cards
		// please note that we iterate over the set from the end to the begin (we use a reverse-iterator),
		// since we want to render animations added later also to be rendered later
		// this is important when we have multiple CardAnimations from the draw card stack to the hand card stack,
		// for example when the player was forced to draw 2 cards
		auto localPlayerAnimations = localPlayer->getCardStack().getCardAnimations();
		for(auto it = localPlayerAnimations.rbegin(); it != localPlayerAnimations.rend(); ++it) {
			auto& animation = *it;

			if(animation.source.get().equalsId(drawStack)) {
				// render cards from draw card stack to hand cards
				float cardStackHeightAddition = CardStackRenderer::ADDITION_PER_CARD * (drawStack.getSize());
				cardInterpolator.interpolateAndRender(animation,
										   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition, 0),
										   DRAW_CARDS_ROTATION,
										   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition + CardRenderer::HEIGHT / 4, CardRenderer::HEIGHT * 0.75f),
										   DRAW_CARDS_ROTATION - glm::vec3(PI / 4, 0, 0),
										   DRAW_CARDS_POSITION + glm::vec3(0, cardStackHeightAddition + CardRenderer::HEIGHT, CardRenderer::HEIGHT * 1.5f),
										   DRAW_CARDS_ROTATION - glm::vec3(PI / 2, 0, 0),
										   HAND_CARDS_LOCAL_POSITION,
										   HAND_CARDS_LOCAL_ROTATION);
			} else if(animation.source.get().equalsId(localPlayer->getTempCardStack())) {
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
		for(auto& animation : localPlayer->getTempCardStack().getCardAnimations()) {
			cardInterpolator.interpolateAndRender(animation,
									   DRAW_CARDS_POSITION + glm::vec3(0, CardStackRenderer::ADDITION_PER_CARD * (drawStack.getSize()), 0),
									   DRAW_CARDS_ROTATION,
									   DRAW_CARDS_POSITION + glm::vec3(0, CardStackRenderer::ADDITION_PER_CARD * (drawStack.getSize()), CardRenderer::HEIGHT),
									   DRAW_CARDS_ROTATION - glm::vec3(PI / 4, 0, 0),
									   DrawnCardRenderer::POSITION,
									   DrawnCardRenderer::ROTATION
			);
		}
	}
	std::optional<int> LocalPlayerRenderer::checkIntersectionWithHandCards(ProxyMauMauGame& game) {
		return intersectionChecker.getIndexOfIntersectedCardInX(game.getLocalPlayer()->getCardStack(), HAND_CARDS_LOCAL_POSITION, HAND_CARDS_LOCAL_ROTATION, FRONT_BACK_OPPONENT_CARDS_WIDTH, CardRenderer::WIDTH, CardRenderer::HEIGHT);
	}
}