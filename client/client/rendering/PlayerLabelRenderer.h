
#pragma once
#include <egui/model/scene/Scene.h>
#include "../model/ProxyPlayer.h"
#include "../gui/PlayerLabel.h"
#include <memory>
#include "Renderer2D.h"
#include "../renderingModel/AvatarTextures.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>

namespace card {
	class PlayerLabelRenderer {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            egui::MasterRenderer& eguiRenderer;
            Renderer2D& renderer2D;

            SimpleTexture textureSkip;
            AvatarTextures avatarTextures;

            egui::Scene scene;
            std::shared_ptr<PlayerLabel> playerLocal;
            std::shared_ptr<PlayerLabel> playerVisAVis;
            std::shared_ptr<PlayerLabel> playerLeft;
            std::shared_ptr<PlayerLabel> playerRight;

            Avatar localAvatar, visAVisAvatar, leftAvatar, rightAvatar;
        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            PlayerLabelRenderer(egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
             void renderLocal(const std::shared_ptr<ProxyPlayer>& participant);
             void renderVisAVis(const std::shared_ptr<ProxyPlayer>& participant);
             void renderLeft(const std::shared_ptr<ProxyPlayer>& participant);
             void renderRight(const std::shared_ptr<ProxyPlayer>& participant);
             void flush();
            
        private:
            void updatePositions();
            void flushText();
            void flushImages();
            void flushImageOfPlayer(const std::shared_ptr<PlayerLabel>& element, Avatar avatar);
            void endFlush();

	};
}