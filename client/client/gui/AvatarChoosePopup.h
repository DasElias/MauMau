#pragma once
#include <egui/model/nodes/UnorganizedParentElement.h>
#include "../renderingModel/AvatarTextures.h"
#include "CategorizedAvatarGrid.h"

namespace card {
	class AvatarChoosePopup : public egui::UnorganizedParentElement {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::function<void(void)> onCloseFunction = [](){};
            std::shared_ptr<CategorizedAvatarGrid> grid;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            AvatarChoosePopup(AvatarTextures& avatarTextures);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void setOnCloseFunction(std::function<void(void)>);
            Avatar getSelectedAvatar() const;
            void setSelectedAvatar(Avatar a);
	};
}