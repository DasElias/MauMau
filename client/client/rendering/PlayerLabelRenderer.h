#pragma once
#include <egui/model/scene/Scene.h>
#include "../model/ParticipantOnClient.h"
#include "../gui/PlayerLabel.h"
#include <memory>
#include "Renderer2D.h"
#include "../renderingModel/SimpleTexture.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>

namespace card {
	class PlayerLabelRenderer {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            egui::MasterRenderer& eguiRenderer;
            Renderer2D& renderer2D;

            egui::Scene scene;

            std::shared_ptr<PlayerLabel> playerVisAVis;
            std::shared_ptr<PlayerLabel> playerLeft;
            std::shared_ptr<PlayerLabel> playerRight;

            SimpleTexture textureVisAVis;
            SimpleTexture textureLeft;
            SimpleTexture textureRight;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            PlayerLabelRenderer(egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
             void renderVisAVis(const std::shared_ptr<ParticipantOnClient>& participant);
             void renderLeft(const std::shared_ptr<ParticipantOnClient>& participant);
             void renderRight(const std::shared_ptr<ParticipantOnClient>& participant);
             void flush();
            
        private:
            void updatePositions();
            void flushText();
            void flushImages();
            void endFlush();

	};
}