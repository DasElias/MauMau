#pragma once
#include "CardRenderer.h"
#include "../model/CardAnimation.h"

namespace card {
	class CardInterpolator {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            CardRenderer& cardRenderer;
            ProjectionMatrix& projectionMatrix;
            Viewport& viewport;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            CardInterpolator(CardRenderer& cardRenderer, ProjectionMatrix& projectionMatrix, Viewport& viewport);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void interpolateAndRender(const CardAnimation& animation, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 middle1Position, glm::vec3 middle1Rotation, glm::vec3 middle2Position, glm::vec3 middle2Rotation, glm::vec3 endPosition, glm::vec3 endRotation, float period1Ratio = 0.33f, float period2Ratio = 0.33f, float period3Ratio = 0.33f);
            void interpolateAndRender(const CardAnimation& animation, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 middlePosition, glm::vec3 middleRotation, glm::vec3 endPosition, glm::vec3 endRotation, float period1Ratio = 0.5f, float period2Ratio = 0.5f);
            void interpolateAndRender(const CardAnimation& animation, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 endPosition, glm::vec3 endRotation);

	};
}