#pragma once
#include <egui/model/nodes/HBox.h>
#include "ToggleSwitch.h"

namespace card {
	class BoolOptionElement : public egui::HBox {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<ToggleSwitch> switchElement;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            BoolOptionElement(std::string labelText);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            bool isToggled() const;
            void set(bool isToggled);
            egui::EventManager<egui::ToggleEvent>& getToggleEventManager();


	};
}