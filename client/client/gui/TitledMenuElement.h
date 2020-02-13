#pragma once
#include "MenuElement.h"
#include <egui/model/nodes/Label.h>
#include <egui/model/nodes/VBox.h>

namespace card {
	class TitledMenuElement : public MenuElement {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static int const FONT_SIZE_PX = 75;
            static float const TITLE_Y_MARGIN_PERCENT_ON_SCREEN;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:    
            std::shared_ptr<egui::Label> title;       

        protected:
            std::shared_ptr<egui::VBox> contentBox;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            TitledMenuElement(std::string title);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void setTitle(std::string title);

        protected:
            egui::Value getYEndOfTitle();
	};
}