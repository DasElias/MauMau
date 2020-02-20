#pragma once
#include <egui/model/nodes/VBox.h>
#include <egui/model/nodes/Label.h>
#include <egui/model/nodes/Button.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include "../renderingModel/AvatarTextures.h"
#include <egui/model/nodeComponents/background/TexturedBackground.h>
#include "ColoredButton.h"

namespace card {
	class ParticipantEntry : public egui::VBox {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        public:
            typedef unsigned int EntryAttributes;
            static EntryAttributes const ROOM_LEADER = 1u << 0;
            static EntryAttributes const LOCAL_PLAYER = 1u << 1;
            static EntryAttributes const AI_PLAYER = 1u << 2;
            static EntryAttributes const CAN_BE_KICKED = 1u << 3;
            static EntryAttributes const CAN_BE_PROMOTED = 1u << 4;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::shared_ptr<egui::Label> usernameLabel;
            std::shared_ptr<egui::Label> descriptionLabel;
            std::shared_ptr<egui::TexturedBackground> avatar;
            std::shared_ptr<ColoredButton> kickBtn;
            std::shared_ptr<ColoredButton> promoteBtn;

            
        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ParticipantEntry(AvatarTextures& avatarTextures, std::string username, Avatar av, EntryAttributes attributes);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void addKickHandler(std::function<void(void)> handler);
            void addPromoteHandler(std::function<void(void)> handler);

        private:
            std::string getDescriptionForAttributes(EntryAttributes attributes);

	};
}