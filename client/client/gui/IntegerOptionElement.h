#pragma once
#include <egui/model/nodes/HBox.h>
#include "IntegerSelector.h"

namespace card {
	class IntegerOptionElement : public egui::HBox {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<IntegerSelector> selector;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            IntegerOptionElement(std::string labelText, int defaultValue, int minValue, int maxValue);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            int getValue() const;
            void setValue(int value);

	};
}