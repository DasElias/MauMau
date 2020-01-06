#pragma once
#include <string>
#include <egui/model/nodes/VBox.h>
#include <egui/model/nodes/Label.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>

namespace card {
	class PlayerLabel : public egui::VBox {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        public:
            static float const WIDTH_RELATIVE_ON_SCREEN;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<egui::Label> playerNameLabel;
            std::shared_ptr<egui::AspectRatioElement> imageElement;
            std::shared_ptr<egui::RelativePositioningOnScreen> positioning;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            PlayerLabel();

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void set(std::string playerName);
            void clear();
            void setPositionOnScreen(float x, float y);
            std::shared_ptr<egui::AspectRatioElement> getImageElement();
	};
}