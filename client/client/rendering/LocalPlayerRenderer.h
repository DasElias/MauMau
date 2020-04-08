#pragma once
#include "../model/ProxyMauMauGame.h"
#include "HandCardStackRenderer.h"
#include "CardInterpolator.h"
#include "../renderingModel/HandCardIntersectionChecker.h"
#include <optional>
#include "../renderingModel/MauMauCardStackMisalignmentGenerator.h"
#include "AnimatorToPlayStack.h"
#include "HandCardsWithAnimationToPlayStackRenderer.h"

namespace card {
	class LocalPlayerRenderer {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        public:
            static float const HOVERED_CARD_Y_ADDITION;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            ProjectionMatrix& projectionMatrix;
            Viewport& viewport;
            CardRenderer& cardRenderer;
            HandCardsWithAnimationToPlayStackRenderer handCardsRenderer;
            AnimatorToPlayStack animatorToPlayStack;

            HandCardIntersectionChecker intersectionChecker;
            CardInterpolator cardInterpolator;

            int intersectedCardIndexInLastFrame = -1;
            long long unixTimeIntersectedCardIndexHasChanged;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            LocalPlayerRenderer(ProjectionMatrix& projectionMatrix, Viewport& viewport, CardRenderer& cardRenderer, CardStackMisalignmentGenerator& misalignmentGenerator);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void render(ProxyMauMauGame& game);
            std::optional<int> checkIntersectionWithHandCards(ProxyMauMauGame& game);

        private:
            void renderHandCards(ProxyMauMauGame& game);
            std::pair<int, float> getIntersectedIndexAndAddition(ProxyMauMauGame& game);
            void renderAnimations(ProxyMauMauGame& data);
            bool shouldDisableCard(ProxyMauMauGame& game, Card c);

            PositionedCardStack getPositionedCardStackOfLocalPlayer(ProxyMauMauGame& game);
	};
}