#pragma once
#include "Renderer2D.h"
#include <optional>
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "../renderingModel/SimpleTexture.h"

namespace card {
	class MauMauPlayerLabelOverlayRenderer {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            Renderer2D& renderer2D;
            float playerLabelWidthRelativeOnScreen;
            float avatarAspectRatio;

            SimpleTexture textureSkip;
            SimpleTexture textureMau;

            std::shared_ptr<egui::AspectRatioElement> skipAnimElement;
            std::shared_ptr<egui::AspectRatioElement> mauAnimElement;
            std::shared_ptr<egui::RelativePositioningOnScreen> basicPositioning;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            MauMauPlayerLabelOverlayRenderer(Renderer2D& renderer2D, float playerLabelWidthRelativeOnScreen, float avatarAspectRatio);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void render(glm::vec2 positionRelativeOnScreen, std::optional<float> percentSkipAnimOrNone, bool renderSkipGrey, std::optional<float> percentMauAnimOrNone);

        private:
            void updateSkipElement(std::optional<float> percentSkipAnimOrNone);
            void updateMauElement(std::optional<float> percentMauAnimOrNone);
            

	};
}