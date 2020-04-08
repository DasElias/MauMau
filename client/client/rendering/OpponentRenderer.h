#pragma once
#include "../model/ProxyMauMauGame.h"
#include <glm/glm.hpp>
#include "HandCardStackRenderer.h"
#include "CardInterpolator.h"
#include "HandCardsWithAnimationToPlayStackRenderer.h"

namespace card {
	class OpponentRenderer {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            HandCardStackRenderer handCardsRenderer;
            CardInterpolator cardInterpolator;
            HandCardsWithAnimationToPlayStackRenderer handCardsWithInterpolationRenderer;

            HandCardStackPositionGenerator handCardStackPositionGenerator;
            ProjectionMatrix& projectionMatrix;
            Viewport& viewport;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            OpponentRenderer(CardRenderer& cardRenderer, ProjectionMatrix& projectionMatrix, Viewport& viewport, CardStackMisalignmentGenerator& playStackMisalignmentGenerator);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void renderHandCardsOfVisAVisOpponent(ProxyMauMauGame& game, ProxyPlayer& player);
            void renderHandCardsOfLeftOpponent(ProxyMauMauGame& game, ProxyPlayer& player);
            void renderHandCardsOfRightOpponent(ProxyMauMauGame& game, ProxyPlayer& player);

            void renderAnimationsFromDrawStackOfVisAVisOpponent(ProxyMauMauGame& game, ProxyPlayer& player);
            void renderAnimationsFromDrawStackOfLeftOpponent(ProxyMauMauGame& game, ProxyPlayer& player);
            void renderAnimationsFromDrawStackOfRightOpponent(ProxyMauMauGame& game, ProxyPlayer& player);

        private:
            void renderAnimationsFromDrawStack(ProxyMauMauGame& game, const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation);
            void renderAnimationsFromDrawStack(ProxyMauMauGame& game, const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, glm::vec3 middlePosition, glm::vec3 middleRotation);



    };
}