#pragma once
#include <egui/model/nodes/HBox.h>
#include "ColoredButton.h"

namespace card {
	class ContinueOptionsBackButtonBar : public egui::HBox {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<ColoredButton> backBtn;
            std::shared_ptr<ColoredButton> optionsBtn;
            std::shared_ptr<ColoredButton> continueBtn;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ContinueOptionsBackButtonBar(std::string backText, std::string optionsText, std::string continueText);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);
            void addOptionsBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);
            void addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);
    };
}