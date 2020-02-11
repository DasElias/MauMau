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
            
        protected:
            std::shared_ptr<egui::VBox> contentBox;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            BasicRoomCreationElement(std::string title);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);
            void addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);

	};
}