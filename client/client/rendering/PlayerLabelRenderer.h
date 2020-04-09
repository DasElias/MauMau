
#pragma once
#include <egui/model/scene/Scene.h>
#include "../model/ProxyPlayer.h"
#include "../gui/PlayerLabel.h"
#include <memory>
#include "Renderer2D.h"
#include "../renderingModel/AvatarTextures.h"
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include "CircleSectorRenderer.h"
#include <boost/optional.hpp>

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
            
            boost::optional<const ProxyPlayer&> playerLocal;
            boost::optional<const ProxyPlayer&> playerVisAVis;
            boost::optional<const ProxyPlayer&> playerLeft;
            boost::optional<const ProxyPlayer&> playerRight;
            
        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            PlayerLabelRenderer(egui::MasterRenderer& eguiRenderer, AvatarTextures& avatarTextures, Renderer2D& renderer2D, CircleSectorRenderer& circleSectorRenderer);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
             void renderLocal(const ProxyPlayer& participant, glm::vec2 positionOnScreen);
             void renderVisAVis(const ProxyPlayer& participant, glm::vec2 positionOnScreen);
             void renderLeft(const ProxyPlayer& participant, glm::vec2 positionOnScreen);
             void renderRight(const ProxyPlayer& participant, glm::vec2 positionOnScreen);
             void flush();
            
        private:
            void renderImpl(const ProxyPlayer& participant, std::shared_ptr<PlayerLabel>& labelElementField, boost::optional<const ProxyPlayer&>& proxyPlayerField);
            void renderCircleSector(const std::shared_ptr<PlayerLabel>& playerLabel, float percentExpired);
            void flushText();
            void flushImages();
            void flushImageOfPlayer(const std::shared_ptr<PlayerLabel>& element, boost::optional<const ProxyPlayer&> participant);
            void endFlush();

	};
}