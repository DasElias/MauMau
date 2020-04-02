#pragma once
#include "../model/ProxyMauMauGame.h"
#include <egui/rendering/MasterRenderer.h>
#include "MauMauButtonRenderer.h"
#include "PassButtonRenderer.h"
#include "DrawnCardRenderer.h"
#include "ChooseCardIndexRenderer.h"

namespace card {
	class ClickableOverlayRenderer {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            CardRenderer& cardRenderer;
            MauMauButtonRenderer mauMauButtonRenderer;
            PassButtonRenderer passButtonRenderer;
            DrawnCardRenderer drawnCardRenderer;
            ChooseCardIndexRenderer chooseCardIndexRenderer;
        
        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ClickableOverlayRenderer(egui::MasterRenderer& eguiRenderer, CardRenderer& cardRenderer, ProjectionMatrix& pm, Viewport& vp, CardIndexTextures& cardIndexTextures);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void renderClickableOverlays(ProxyMauMauGame& game);

            void discardMouseEventsForChooseIndexRenderer();

            void addMauBtnClickHandler(std::function<void(void)> handler);
            void addPassBtnClickHandler(std::function<void(void)> handler);
            void addPlayDrawnCardClickHandler(std::function<void(void)> handler);
            void addTakeDrawnCardIntoHandCardsClickHandler(std::function<void(void)> handler);
            void addChooseCardIndexHandler(std::function<void(CardIndex)> handler);

        private:
            void renderMauBtnIfGameHasntEnded(ProxyMauMauGame& game, bool suppressMouseClick);
            void renderPassBtnIfGameHasntEnded(ProxyMauMauGame& game);
            void renderDrawnCardOverlayIfGameHasntEnded(ProxyMauMauGame& game, bool suppressMouseClick);
            void renderChooseColorOverlayIfGameHasntEnded(ProxyMauMauGame& game);


	};
}