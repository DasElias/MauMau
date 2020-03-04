#pragma once
#include "ParticleRenderer.h"
#include "Renderer2D.h"
#include "../renderingModel/particles/ParticleManager.h"
#include "../renderingModel/particles/FireworkParticleSystem.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/scene/Scene.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include "../gui/GameEndElement.h"

namespace card {
	class GameEndRenderer {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            Viewport fireworkViewport;
            ParticleRenderer& particleRenderer;
            ParticleManager particleManager;
            std::vector<std::shared_ptr<FireworkParticleSystem>> allParticleSystems;

            egui::MasterRenderer& eguiRenderer;
            Renderer2D& renderer2D;

            bool field_isAnimationActive = false;
            std::shared_ptr<GameEndElement> element;
            egui::Scene scene;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            GameEndRenderer(ParticleRenderer& particleRenderer, egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D, std::function<void(void)> continueHandler);

        // ----------------------------------------------------------------------
        // ---------------------------STATIC-METHODS-----------------------------
        // ----------------------------------------------------------------------
        private:   
            static ParticleTexture loadTexture(std::string fileName);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void updateAndRender(float delta, std::string winnerUsername, ProjectionMatrix& projectionMatrix);
            void startAnimation();
            void endAnimation();
            bool isAnimationActive() const;

        private:
            std::shared_ptr<FireworkParticleSystem> getRandomParticleSystem();
            void generateParticlesRecursive();
            void generateParticlesInMs(std::shared_ptr<FireworkParticleSystem> ps, glm::vec2 systemCenter, int& delay);

	};
}