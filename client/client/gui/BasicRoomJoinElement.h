#pragma once
#include "TitledMenuElement.h"
#include "JoinRoomButtonBar.h"
#include <egui/model/nodes/VBox.h>

namespace card {
	class BasicRoomJoinElement : public TitledMenuElement {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<JoinRoomButtonBar> buttonBar;
            
        protected:
            std::shared_ptr<egui::VBox> contentBox;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            BasicRoomJoinElement(std::string title);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void addBackBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);
            void addContinueBtnEventHandler(egui::FunctionWrapper<egui::ActionEvent> handler);

	};
}