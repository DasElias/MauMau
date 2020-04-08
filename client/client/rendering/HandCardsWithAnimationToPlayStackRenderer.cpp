#include "HandCardsWithAnimationToPlayStackRenderer.h"
#include "HandCardsWithAnimationToPlayStackRenderer.h"
#include "HandCardsWithAnimationToPlayStackRenderer.h"
#include "HandCardsWithAnimationToPlayStackRenderer.h"
#include <functional>
#include "../renderingModel/AnimationsToPlayStackFilter.h"

namespace card {
	HandCardsWithAnimationToPlayStackRenderer::HandCardsWithAnimationToPlayStackRenderer(CardRenderer& cardRenderer, ProjectionMatrix& pm, Viewport& vp, CardStackMisalignmentGenerator& misalignmentGenerator) :
			cardRenderer(cardRenderer),
			projectionMatrix(pm),
			viewport(vp),
			animatorToPlayStack(cardRenderer, projectionMatrix, viewport, misalignmentGenerator) {
	}
	void HandCardsWithAnimationToPlayStackRenderer::renderHandCardsOfLocalPlayer(std::vector<PositionedCard> positionedHandCards, const CardAnimator& handCardStack, ProxyMauMauGame& game, std::function<bool(Card)> shouldDisable) {
		auto animationsToPlayCardStack = AnimationsToPlayStackFilterer::getAnimationsFromParticularHandCards(game, handCardStack);		
		auto animateFunction = [&](CardAnimation a) {
			animatorToPlayStack.animateFromLocalPlayerHandCards(a, game.getPlayStack());
		};
		renderImpl(positionedHandCards, animationsToPlayCardStack, animateFunction, shouldDisable);
	}
	void HandCardsWithAnimationToPlayStackRenderer::renderHandCardsOfVisavisOpponent(std::vector<PositionedCard> positionedHandCards, const CardAnimator& handCardStack, ProxyMauMauGame& game, std::function<bool(Card)> shouldDisable) {
		auto animationsToPlayCardStack = AnimationsToPlayStackFilterer::getAnimationsFromParticularHandCards(game, handCardStack);
		auto animateFunction = [&](CardAnimation a) {
			animatorToPlayStack.animateFromVisavisOpponent(a, game.getPlayStack());
		};
		renderImpl(positionedHandCards, animationsToPlayCardStack, animateFunction, shouldDisable);
	}
	void HandCardsWithAnimationToPlayStackRenderer::renderHandCardsOfLeftOpponent(std::vector<PositionedCard> positionedHandCards, const CardAnimator& handCardStack, ProxyMauMauGame& game, std::function<bool(Card)> shouldDisable) {
		auto animationsToPlayCardStack = AnimationsToPlayStackFilterer::getAnimationsFromParticularHandCards(game, handCardStack);
		auto animateFunction = [&](CardAnimation a) {
			animatorToPlayStack.animateFromLeftOpponent(a, game.getPlayStack());
		};
		renderImpl(positionedHandCards, animationsToPlayCardStack, animateFunction, shouldDisable);
	}
	void HandCardsWithAnimationToPlayStackRenderer::renderHandCardsOfRightOpponent(std::vector<PositionedCard> positionedHandCards, const CardAnimator& handCardStack, ProxyMauMauGame& game, std::function<bool(Card)> shouldDisable) {
		auto animationsToPlayCardStack = AnimationsToPlayStackFilterer::getAnimationsFromParticularHandCards(game, handCardStack);
		auto animateFunction = [&](CardAnimation a) {
			animatorToPlayStack.animateFromRightOpponent(a, game.getPlayStack());
		};
		renderImpl(positionedHandCards, animationsToPlayCardStack, animateFunction, shouldDisable);
	}
	void HandCardsWithAnimationToPlayStackRenderer::renderImpl(const std::vector<PositionedCard>& positionedHandCards, std::vector<CardAnimation>& animationsToPlayCardStack, std::function<void(CardAnimation)> animateFunc, std::function<bool(Card)> shouldDisableFunc) {
		for(int i = 0; i < positionedHandCards.size(); i++) {
			PositionedCard handCard = positionedHandCards.at(i);
			animationsToPlayCardStack.erase(std::remove_if(animationsToPlayCardStack.begin(), animationsToPlayCardStack.end(), [&](auto& animatedCard) {
				if(animatedCard.indexInSourceStack == i) {
					animateFunc(animatedCard);
					return true;
				} else return false;
			}), animationsToPlayCardStack.end());

			bool shouldRenderHandCardInGreyScale = shouldDisableFunc(handCard.getCard());
			cardRenderer.renderInNextPass(handCard, projectionMatrix, viewport, shouldRenderHandCardInGreyScale);
		}

		// now we have to proceed the remaining animations (a card which index has been greater than positionedHandCards.size() now is)
		for(auto& remainingAnimation : animationsToPlayCardStack) {
			animateFunc(remainingAnimation);
		}
	}
}