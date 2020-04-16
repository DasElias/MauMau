#pragma once
#include <egui/model/nodes/VBox.h>
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodes/Label.h>
#include "../renderingModel/AvatarTextures.h"
#include <shared/model/Avatar.h>
#include "AvatarGrid.h"

namespace card {
	class CategorizedAvatarGrid : public egui::VBox {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<egui::Label> uncategorizedLabel;
            std::shared_ptr<AvatarGrid> uncategorizedGrid;

            std::shared_ptr<egui::Label> maleLabel;
            std::shared_ptr<AvatarGrid> maleGrid;

            std::shared_ptr<egui::Label> femaleLabel;
            std::shared_ptr<AvatarGrid> femaleGrid;

            Avatar selectedAvatar;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            CategorizedAvatarGrid(AvatarTextures& avatarTextures, Avatar chosenAvatar);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            Avatar getSelectedAvatar() const;
            void setSelectedAvatar(Avatar a);

        private:
            void formatLabel(std::shared_ptr<egui::Label> lbl);
	};
}