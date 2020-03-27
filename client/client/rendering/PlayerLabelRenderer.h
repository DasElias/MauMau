
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
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        public:
            static int const CIRCLE_DIAMETER;
            static int const CIRCLE_RADIUS;
            static int const CIRCLE_CENTER_OFFSET_PX_X;
            static int const CIRCLE_CENTER_OFFSET_PX_Y;
            
        private:
            static glm::vec4 const CIRCLE_SECTOR_COLOR;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            egui::MasterRenderer& eguiRenderer;
            Renderer2D& renderer2D;
            CircleSectorRenderer& circleSectorRenderer;

            AvatarTextures& avatarTextures;

            egui::Scene scene;
            std::shared_ptr<PlayerLabel> labelElementForLocal;
            std::shared_ptr<PlayerLabel> labelElementForVisAVis;
            std::shared_ptr<PlayerLabel> labelElementForLeft;
            std::shared_ptr<PlayerLabel> labelElementForRight;
            
            std::weak_ptr<ProxyPlayer> playerLocal;
            std::weak_ptr<ProxyPlayer> playerVisAVis;
            std::weak_ptr<ProxyPlayer> playerLeft;
            std::weak_ptr<ProxyPlayer> playerRight;
            
        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            PlayerLabelRenderer(egui::MasterRenderer& eguiRenderer, AvatarTextures& avatarTextures, Renderer2D& renderer2D, CircleSectorRenderer& circleSectorRenderer);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
             void renderLocal(const std::shared_ptr<ProxyPlayer>& participant, glm::vec2 positionOnScreen);
             void renderVisAVis(const std::shared_ptr<ProxyPlayer>& participant, glm::vec2 positionOnScreen);
             void renderLeft(const std::shared_ptr<ProxyPlayer>& participant, glm::vec2 positionOnScreen);
             void renderRight(const std::shared_ptr<ProxyPlayer>& participant, glm::vec2 positionOnScreen);
             void flush();
            
        private:
            void renderImpl(const std::shared_ptr<ProxyPlayer>& participant, std::shared_ptr<PlayerLabel>& labelElementField, std::weak_ptr<ProxyPlayer>& proxyPlayerField);
            void renderCircleSector(const std::shared_ptr<PlayerLabel>& playerLabel, float percentExpired);
            void flushText();
            void flushImages();
            void flushImageOfPlayer(const std::shared_ptr<PlayerLabel>& element, std::weak_ptr<ProxyPlayer> participant);
            void endFlush();

	};
}