#pragma once
#include <egui/model/nodes/VBox.h>
#include <egui/model/nodes/InputField.h>
#include <egui/model/nodes/Label.h>

namespace card {
	class LabeledInputField : public egui::VBox {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static int const TITLE_FONT_SIZE_PX = 30;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
           // ----------------------------------------------------------------------
        private:
            std::shared_ptr<egui::InputField> inputField;
            std::shared_ptr<egui::Label> title;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            LabeledInputField(std::string title, egui::Color fontColor);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            std::shared_ptr<egui::InputField> getInputFieldImpl();
            std::string getText() const;
	};
}