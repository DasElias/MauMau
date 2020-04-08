#pragma once
#include "AnimatorToPlayStack.h"
#include "../model/ProxyMauMauGame.h"

namespace card {
	class HandCardsWithAnimationToPlayStackRenderer {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            CardRenderer& cardRenderer;
            ProjectionMatrix& projectionMatrix;
            Viewport& viewport;
            AnimatorToPlayStack animatorToPlayStack;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            HandCardsWithAnimationToPlayStackRenderer(CardRenderer& cardRenderer, ProjectionMatrix& pm, Viewport& vp, CardStackMisalignmentGenerator& misalignmentGenerator);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
           void renderHandCardsOfLocalPlayer(std::vector<PositionedCard> positionedHandCards, const CardAnimator& handCardStack, ProxyMauMauGame& game, std::function<bool(Card)> shouldDisable = [](Card){return false;});
           void renderHandCardsOfVisavisOpponent(std::vector<PositionedCard> positionedHandCards, const CardAnimator& handCardStack, ProxyMauMauGame& game, std::function<bool(Card)> shouldDisable = [](Card) {return false; });
           void renderHandCardsOfLeftOpponent(std::vector<PositionedCard> positionedHandCards, const CardAnimator& handCardStack, ProxyMauMauGame& game, std::function<bool(Card)> shouldDisable = [](Card) {return false; });
           void renderHandCardsOfRightOpponent(std::vector<PositionedCard> positionedHandCards, const CardAnimator& handCardStack, ProxyMauMauGame& game, std::function<bool(Card)> shouldDisable = [](Card) {return false; });

        private:
            void renderImpl(const std::vector<PositionedCard>& positionedHandCards, std::vector<CardAnimation>& animationsToPlayCardStack, std::function<void(CardAnimation)> animateFunc, std::function<bool(Card)> shouldDisableFunc);
	};
}