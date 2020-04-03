#pragma once
#include <egui/model/scene/Scene.h>
#include <egui/model/nodes/Button.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "../renderingModel/IngameButton.h"
#include "../renderingModel/WorldToScreenConverter.h"
#include "../renderingModel/MauMauButtonPositionGenerator.h"

namespace card {
	class PassButtonRenderer {
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
            MauMauButtonPositionGenerator mauMauButtonPositionGenerator;
            std::shared_ptr<egui::RelativePositioningOnScreen> passBtnPositioning;
            std::shared_ptr<IngameButton> passBtn;
            egui::Scene scene;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            PassButtonRenderer(egui::MasterRenderer& eguiRenderer, ProjectionMatrix& pm, Viewport& vp);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void clearPreviousMouseEvents();
            void render();
            void addClickHandler(std::function<void(void)> passFunction);

        private:
            void updateBtnPosition();


	};
}