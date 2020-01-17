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

namespace card {
	class CardIndexRenderer {     
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            Renderer2D& renderer2D;
            CardIndexTextures cardIndexTextures;
            SimpleTexture letteringTexture;

            std::shared_ptr<egui::VBox> parentElement;
            std::shared_ptr<egui::Label> title;
            std::shared_ptr<egui::AspectRatioElement> imageElement;
            egui::Scene scene;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            CardIndexRenderer(Renderer2D& renderer2D);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void renderCardIndexForNextCard(CardIndex indexToRender);

	};
}