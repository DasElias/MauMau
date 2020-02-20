#pragma once
#include "MenuElement.h"
#include "ColoredButton.h"
#include "ParticipantEntry.h"
#include "../model/ProxyRoom.h"
#include <egui/model/nodes/VBox.h>
#include <egui/model/nodes/HBox.h>
#include <egui/model/nodes/Label.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include "EditOptionsElement.h"
#include "ViewOptionsElement.h"

namespace card {
	class ParticipantsOverviewElement : public MenuElement {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            AvatarTextures& avatarTextures;

            std::shared_ptr<egui::UnorganizedParentElement> blackOverlay;
            std::shared_ptr<EditOptionsElement> editOptionsElement;
            std::shared_ptr<ViewOptionsElement> viewOptionsElement;

            std::shared_ptr<egui::Label> roomCodeLabel;
            std::shared_ptr<egui::AspectRatioElement> leaveRoomBtn;
            std::shared_ptr<egui::AspectRatioElement> editOptionsBtn;
            std::shared_ptr<egui::AspectRatioElement> viewOptionsBtn;
            std::shared_ptr<egui::AspectRatioElement> addAiPlayerBtn;
            std::shared_ptr<egui::AspectRatioElement> startGameBtn;
            std::shared_ptr<egui::HBox> participantsBox;
            std::shared_ptr<egui::HBox> buttonBox;


            std::function<void(std::shared_ptr<ParticipantOnClient>)> kickHandler = [](std::shared_ptr<ParticipantOnClient>) {};
            std::function<void(std::shared_ptr<ParticipantOnClient>)> promoteHandler = [](std::shared_ptr<ParticipantOnClient>) {};


        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ParticipantsOverviewElement(AvatarTextures& avatarTextures);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void update(ProxyRoom& room);
            void addOptionsChangeEventHandler(std::function<void(RoomOptions)> handler);
            void addAiPlayerJoinHandler(std::function<void(void)> handler);
            void addGameStartHandler(std::function<void(void)> handler);
            void addLeaveRoomHandler(std::function<void(void)> handler);

            void setKickEventHandler(std::function<void(std::shared_ptr<ParticipantOnClient>)> handler);
            void setPromoteEventHandler(std::function<void(std::shared_ptr<ParticipantOnClient>)> handler);

        private:
            void rebuildParticipants(ProxyRoom& room);
            std::shared_ptr<egui::AspectRatioElement> addButton(std::shared_ptr<egui::Image> texImage, std::shared_ptr<egui::Image> hoverImage);
	};
}