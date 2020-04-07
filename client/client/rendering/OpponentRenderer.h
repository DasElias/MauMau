#pragma once
#include "../model/ProxyMauMauGame.h"
#include <glm/glm.hpp>
#include "HandCardStackRenderer.h"
#include "CardInterpolator.h"

namespace card {
	class OpponentRenderer {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            HandCardStackRenderer handCardsRenderer;
            CardInterpolator cardInterpolator;
            ProjectionMatrix& projectionMatrix;
            Viewport& viewport;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            OpponentRenderer(CardRenderer& cardRenderer, ProjectionMatrix& projectionMatrix, Viewport& viewport);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void renderOpponentIfHasValue_x(std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation);
            void renderOpponent_x(PositionedCardStack positionedCardStack);
            void renderOpponentIfHasValue_z(std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation);
            void renderOpponent_z(PositionedCardStack positionedCardStack);

            void renderDrawedCardAnimationsOfOpponentIfHasValue(ProxyMauMauGame& game, std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation);
            void renderDrawedCardAnimationsOfOpponent(ProxyMauMauGame& game, const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation);
            void renderDrawedCardAnimationsOfOpponentIfHasValue(ProxyMauMauGame& game, std::size_t index, std::array<std::shared_ptr<ProxyPlayer>, 3>& opponentsOrNullInCwOrder, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, glm::vec3 middlePosition, glm::vec3 middleRotation);
            void renderDrawedCardAnimationsOfOpponent(ProxyMauMauGame& game, const CardAnimator& handCardStack, glm::vec3 handCardsPosition, glm::vec3 handCardsRotation, glm::vec3 middlePosition, glm::vec3 middleRotation);
    };
}