#pragma once
#include <egui/model/nodes/AspectRatioElement.h>

#include <egui/model/nodes/UnorganizedParentElement.h>
#include "../renderingModel/AvatarTextures.h"
#include <shared/model/Avatar.h>

namespace card {
	class AvatarGrid : public egui::UnorganizedParentElement {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static int const AVATARS_PER_ROW = 8;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::vector<std::shared_ptr<egui::AspectRatioElement>> images;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            AvatarGrid(AvatarTextures& avatarTextures, std::vector<Avatar> avatarsInGrid);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:


	};
}