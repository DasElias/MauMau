#pragma once
#include "TitledMenuElement.h"
#include "ContinueBackButtonBar.h"
#include <egui/model/nodes/VBox.h>

namespace card {
	class BasicRoomJoinElement : public TitledMenuElement {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<ContinueBackButtonBar> buttonBar;
           

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            BasicRoomJoinElement(std::string title, std::string continueBtnText);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);
            void addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);

	};
}