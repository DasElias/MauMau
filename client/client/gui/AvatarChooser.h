#pragma once
#include <egui/model/nodes/AspectRatioElement.h>
#include "../renderingModel/AvatarTextures.h"
#include <egui/model/nodeComponents/background/TexturedBackground.h>

namespace card {
	class AvatarChooser : public egui::AspectRatioElement {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            AvatarTextures& avatarTextures;
            Avatar chosenAvatar;
            std::shared_ptr<egui::TexturedBackground> avatarBackground;

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

	};
}