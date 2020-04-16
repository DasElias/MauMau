#pragma once
#include <egui/model/nodes/AspectRatioElement.h>
#include "../renderingModel/AvatarTextures.h"
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include <egui/model/nodes/HBox.h>
#include <egui/model/nodes/VBox.h>
#include "AvatarChoosePopup.h"

namespace card {
	class AvatarChooser : public egui::VBox {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            AvatarTextures& avatarTextures;
            Avatar chosenAvatar;
            std::shared_ptr<egui::TexturedBackground> avatarBackground;
            std::shared_ptr<AvatarChoosePopup> popup;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            AvatarChooser(AvatarTextures& avatarTextures, Avatar defaultAvatar);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            Avatar getSelectedAvatar() const;
            void setSelectedAvatar(Avatar a);

	};
}