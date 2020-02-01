
#pragma once
#include <egui/model/scene/Scene.h>
#include "../model/ProxyPlayer.h"
#include "../gui/PlayerLabel.h"
#include <memory>
#include "Renderer2D.h"
#include "../renderingModel/AvatarTextures.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "CircleSectorRenderer.h"

namespace card {
	class PlayerLabelRenderer {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            egui::MasterRenderer& eguiRenderer;
            Renderer2D& renderer2D;
            CircleSectorRenderer& circleSectorRenderer;

            SimpleTexture textureSkip;
            SimpleTexture textureMau;
            AvatarTextures avatarTextures;

            egui::Scene scene;
            std::shared_ptr<PlayerLabel> labelElementForLocal;
            std::shared_ptr<PlayerLabel> labelElementForVisAVis;
            std::shared_ptr<PlayerLabel> labelElementForLeft;
            std::shared_ptr<PlayerLabel> labelElementForRight;
            
            std::shared_ptr<ProxyPlayer> playerLocal;
            std::shared_ptr<ProxyPlayer> playerVisAVis;
            std::shared_ptr<ProxyPlayer> playerLeft;
            std::shared_ptr<ProxyPlayer> playerRight;
            
        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            PlayerLabelRenderer(egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D, CircleSectorRenderer& circleSectorRenderer);

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
            void renderImpl(const std::shared_ptr<ProxyPlayer>& participant, std::shared_ptr<PlayerLabel>& labelElementField, std::shared_ptr<ProxyPlayer>& proxyPlayerField);
            void renderCircleSector(const std::shared_ptr<PlayerLabel>& playerLabel, float percentExpired);
            void updatePositions();
            void flushText();
            void flushImages();
            void flushImageOfPlayer(const std::shared_ptr<PlayerLabel>& element, const std::shared_ptr<ProxyPlayer>& participant);
            void endFlush();

	};
}