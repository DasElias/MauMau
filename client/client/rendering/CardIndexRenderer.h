#pragma once
#include "Renderer2D.h"
#include "../renderingModel/SimpleTexture.h"
#include <array>
#include <egui/model/scene/Scene.h>
#include <shared/model/CardIndex.h>
#include <egui/model/nodes/VBox.h>
#include <egui/model/nodes/Label.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "../renderingModel/CardIndexTextures.h"
#include "../renderingModel/WorldToScreenConverter.h"

namespace card {
	class CardIndexRenderer {  
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static int const SPACE_BETWEEN_TITLE_AND_IMAGE_PX;
            static int const TITLE_STRETCH;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            Renderer2D& renderer2D;
            WorldToScreenConverter worldToScreenConverter;
            CardIndexTextures& cardIndexTextures;
            SimpleTexture letteringTexture;

            std::shared_ptr<egui::RelativePositioningOnScreen> parentElementPositioning;
            std::shared_ptr<egui::VBox> parentElement;
            std::shared_ptr<egui::AspectRatioElement> title;
            std::shared_ptr<egui::AspectRatioElement> imageElement;
            egui::Scene scene;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            CardIndexRenderer(Renderer2D& renderer2D, CardIndexTextures& cardIndexTextures, ProjectionMatrix& projectionMatrix, Viewport& viewport);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void renderCardIndexForNextCard(CardIndex indexToRender);

        private:
            void updateParentElementPositioning();
            void updateParentElementWidth();
            float getDistanceBetweenCardStacks();

	};
}