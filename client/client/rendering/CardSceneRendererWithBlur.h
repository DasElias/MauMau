#pragma once
#include "CardSceneRenderer.h"
#include "GaussianBlurPostProcessor.h"
#include <memory>
#include <boost/optional.hpp>
#include "../renderingModel/FrameBufferObject.h"

namespace card {
	class CardSceneRendererWithBlur {
        enum class FboStatus {
            SHOULD_UPDATE,
            RENDER_REGULARLY,
            DONT_RENDER
        };

        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static int const RESIZING_FBO_DELAY_MS = 250;
            static int const SAMPLES = 2;
        
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            CardSceneRenderer& cardSceneRenderer;
            GaussianBlurPostProcessor blurPostProcessor;
            std::unique_ptr<FrameBufferObject> outputFbo, multisampleFbo;

            int lastFrameWindowWidth;
            int lastFrameWindowHeight;
            long long lastTimeResizing;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            CardSceneRendererWithBlur(CardSceneRenderer& cardSceneRenderer);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void renderWithGaussianBlur(float deltaSeconds);
            void render(float deltaSeconds);

        private:
            FboStatus getFboStatus();
            void renderImpl(float deltaSeconds, bool shouldUpdateFbo, boost::optional<FrameBufferObject&> resolveTo);
            std::unique_ptr<FrameBufferObject> createFbo(int samples = 0);
	};
}