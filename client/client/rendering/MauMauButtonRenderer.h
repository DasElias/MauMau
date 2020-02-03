#pragma once
#include <egui/model/scene/Scene.h>
#include <egui/model/nodes/Button.h>
#include <egui/model/nodes/AspectRatioElement.h>

namespace card {
	class MauMauButtonRenderer {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static float const MAU_MAU_BTN_WIDTH_PERCENT;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            egui::MasterRenderer& eguiRenderer;
            std::shared_ptr<egui::AspectRatioElement> mauMauButton;
            egui::Scene scene;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            MauMauButtonRenderer(egui::MasterRenderer& eguiRenderer, std::function<void(void)> mauMauFunction);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void clearPreviousMouseEvents();
            void render(bool canMau);

	};
}