#pragma once
#include "TitledMenuElement.h"
#include "ContinueOptionsBackButtonBar.h"
#include <egui/model/nodes/VBox.h>

namespace card {
	class BasicRoomCreationElement : public TitledMenuElement {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<ContinueOptionsBackButtonBar> buttonBar;
           

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            BasicRoomCreationElement(std::string title, std::string continueText);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);
            void addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);

        protected:
            void addOptionsBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);

	};
}