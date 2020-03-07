#pragma once
#include "../model/ProxyMauMauGame.h"
#include "HandCardStackRenderer.h"
#include "CardInterpolator.h"
#include "../renderingModel/HandCardIntersectionChecker.h"
#include <optional>

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

            HandCardIntersectionChecker intersectionChecker;
            HandCardStackRenderer handCardStackRenderer;
            CardInterpolator cardInterpolator;

            int intersectedCardIndexInLastFrame = -1;
            long long unixTimeIntersectedCardIndexHasChanged;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            LocalPlayerRenderer(ProjectionMatrix& projectionMatrix, Viewport& viewport, CardRenderer& cardRenderer);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void render(ProxyMauMauGame& game);
            std::optional<int> checkIntersectionWithHandCards(ProxyMauMauGame& game);

        private:
            void renderHandCards(ProxyMauMauGame& game);
            void renderAnimations(ProxyMauMauGame& data);
	};
}