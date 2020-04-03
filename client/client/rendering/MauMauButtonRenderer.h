#pragma once
#include <egui/model/scene/Scene.h>
#include <egui/model/nodes/Button.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include "../renderingModel/IngameButton.h"
#include "../renderingModel/WorldToScreenConverter.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "../renderingModel/MauMauButtonPositionGenerator.h"

namespace card {
	class MauMauButtonRenderer {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static float const BTN_WIDTH_PERCENT;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            egui::MasterRenderer& eguiRenderer;
            MauMauButtonPositionGenerator mauMauPositionGenerator;
            std::shared_ptr<IngameButton> mauMauBtn;
            std::shared_ptr<egui::RelativePositioningOnScreen> mauMauBtnPositioning;
            egui::Scene scene;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            MauMauButtonRenderer(egui::MasterRenderer& eguiRenderer, ProjectionMatrix& pm, Viewport& vp);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void clearPreviousMouseEvents();
            void render(bool canMau);
            void addClickHandler(std::function<void(void)> mauMauFunction);

        private:
            void updateBtnPosition();

	};
}