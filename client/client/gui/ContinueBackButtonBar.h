#pragma once
#include <egui/model/nodes/HBox.h>
#include "ColoredButton.h"

namespace card {
	class ContinueBackButtonBar : public egui::HBox {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<ColoredButton> backBtn;
            std::shared_ptr<ColoredButton> continueBtn;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ContinueBackButtonBar(std::string backBtnText, std::string continueBtnText);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);
            void addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);
    };
}