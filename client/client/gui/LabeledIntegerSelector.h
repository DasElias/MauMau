#pragma once
#include <egui/model/nodes/VBox.h>
#include "IntegerSelector.h"
#include <egui/model/nodes/Label.h>

namespace card {
	class LabeledIntegerSelector : public egui::VBox {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static int const TITLE_FONT_SIZE_PX = 25;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
           // ----------------------------------------------------------------------
        private:
            std::shared_ptr<IntegerSelector> inputField;
            std::shared_ptr<egui::Label> title;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            LabeledIntegerSelector(std::string title, int defaultValue, int minValue, int maxValue, egui::Color fontColor);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            std::shared_ptr<IntegerSelector> getIntegerSelectorImpl();
            int getValue() const;
	};
}