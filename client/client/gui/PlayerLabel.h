#pragma once
#include <string>
#include <egui/model/nodes/VBox.h>
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodes/Label.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <optional>
#include <glm/glm.hpp>

namespace card {
	class PlayerLabel : public egui::UnorganizedParentElement {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        public:
            static float const IMAGE_WIDTH_RELATIVE_ON_SCREEN;
            static int const SKIP_IMAGE_WIDTH_ADDITION;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<egui::VBox> rootElement;
            std::shared_ptr<egui::Label> playerNameLabel;
            std::shared_ptr<egui::AspectRatioElement> imageElement;
            std::shared_ptr<egui::RelativePositioningOnScreen> positioning;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            PlayerLabel(float avatarAspectRatio);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void set(std::string playerName);
            void clear();
            void setPositionOnScreen(float x, float y);
            void setPositionOnScreen(glm::vec2 pos);
            std::shared_ptr<egui::AspectRatioElement> getImageElement(); 

        private:
            void setOwnPositioning(std::shared_ptr<egui::Positioning> positioning) override;


	};
}