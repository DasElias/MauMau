#pragma once
#include "BasicOptionsElement.h"

namespace card {
	class EditOptionsElement : public BasicOptionsElement {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<ContinueBackButtonBar> buttonBar;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            EditOptionsElement();

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);
            void addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);

	};
}