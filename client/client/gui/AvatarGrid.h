#pragma once
#include <egui/model/nodes/AspectRatioElement.h>

#include <egui/model/nodes/UnorganizedParentElement.h>
#include "../renderingModel/AvatarTextures.h"
#include <shared/model/Avatar.h>
#include <optional>

namespace card {
	class AvatarGrid : public egui::UnorganizedParentElement {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static int const AVATARS_PER_ROW = 12;
            static float const WIDTH_PER_AVATAR;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<egui::AspectRatioElement> selection;
            std::shared_ptr<egui::AspectRatioElement> hover;

            std::vector<std::pair<Avatar, std::shared_ptr<egui::AspectRatioElement>>> images;
            AvatarTextures& avatarTextures;
            std::optional<Avatar> selectedAvatar;
            std::function<void(Avatar)> onSelectHandler = [](Avatar){};

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            AvatarGrid(AvatarTextures& avatarTextures, std::vector<Avatar> avatarsInGrid, std::optional<Avatar> selectedAvatar);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            float getHeightPerAvatar() const;

            std::optional<Avatar> getSelectedAvatar() const;
            void clearSelectedAvatar();
            void setOnSelectHandler(std::function<void(Avatar)> handler);

        private:
            void updateOverlay(std::optional<Avatar> avatar, std::shared_ptr<egui::AspectRatioElement> overlay);
            std::shared_ptr<egui::Positioning> genPositioningForAvatar(std::size_t index);
	};
}