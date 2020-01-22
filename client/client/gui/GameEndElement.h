#pragma once
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodes/Label.h>
#include <egui/model/nodes/HBox.h>
#include <egui/model/nodes/Button.h>
#include "ColoredButton.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>

namespace card {
	class GameEndElement : public egui::UnorganizedParentElement {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        public:
            static int const BG_OPACITY_ANIMATION_DURATION_MS = 1500;
            static int const FONT_OPACITY_ANIMATION_DURATION_MS = 1000;
            static int const ANIMATIONS_COMPLETED_DURATION = BG_OPACITY_ANIMATION_DURATION_MS + FONT_OPACITY_ANIMATION_DURATION_MS;

        private:
            static float const BG_MIN_OPACITY;
            static float const BG_MAX_OPACITY;
            static float const FONT_MIN_OPACITY;
            static float const FONT_MAX_OPACITY;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            float bgOpacity;
            float fontOpacity;
            std::shared_ptr<egui::ColoredBackground> background;
            std::shared_ptr<egui::Label> titleLabel;
            std::shared_ptr<egui::Label> winnerLabel;

            std::shared_ptr<egui::HBox> buttonBar;
            std::shared_ptr<ColoredButton> continueBtn;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            GameEndElement();

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void startAnimation();
            void setOnContinueBtn(egui::FunctionWrapper<egui::ActionEvent> evtHandler);
            void update(float deltaSeconds, std::string winnerUsername);

        private:
            void updateBg(float deltaMs);
            void updateFonts(float deltaMs);

	};
}