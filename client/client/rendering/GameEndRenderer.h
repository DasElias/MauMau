#pragma once
#include "ParticleRenderer.h"
#include "Renderer2D.h"
#include "../model/particles/ParticleManager.h"
#include "../model/particles/FireworkParticleSystem.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/scene/Scene.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>

namespace card {
	class GameEndRenderer {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static float const START_BG_OPACITY;
            static float const END_BG_OPACITY;
            static int const BG_OPACITY_ANIMATION_DURATION_MS = 1500;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            Viewport fireworkViewport;
            ParticleRenderer& particleRenderer;
            ParticleManager particleManager;
            std::shared_ptr<FireworkParticleSystem> redParticleSystem;
            std::shared_ptr<FireworkParticleSystem> greenParticleSystem;

            egui::MasterRenderer& eguiRenderer;
            Renderer2D& renderer2D;

            bool isAnimationActive = false;
            float bgOpacity;
            std::shared_ptr<egui::UnorganizedParentElement> bgElement;
            std::shared_ptr<egui::ColoredBackground> bgElementColor;
            egui::Scene scene;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            GameEndRenderer(ParticleRenderer& particleRenderer, egui::MasterRenderer& eguiRenderer, Renderer2D& renderer2D);

        // ----------------------------------------------------------------------
        // ---------------------------STATIC-METHODS-----------------------------
        // ----------------------------------------------------------------------
        private:   
            static ParticleTexture loadTexture(std::string fileName);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void updateAndRender(float delta, ProjectionMatrix& projectionMatrix);
            void startAnimation();

        private:
            void generateParticlesInMs(std::shared_ptr<FireworkParticleSystem> ps, glm::vec3 center, int delayMs);

	};
}