#pragma once
#include "CardInterpolator.h"
#include "../renderingModel/MauMauCardStackMisalignmentGenerator.h"
#include "../renderingModel/HandCardStackPositionGenerator.h"

namespace card {
	class AnimatorToPlayStack {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            CardInterpolator cardInterpolator;
            CardStackMisalignmentGenerator& misalignmentGenerator;
            HandCardStackPositionGenerator handCardPositionGenerator;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            AnimatorToPlayStack(CardRenderer& cardRenderer, ProjectionMatrix& pm, Viewport& vp, CardStackMisalignmentGenerator& misalignmentGenerator);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void animateFromLocalPlayerHandCards(const CardAnimation& animation, const CardAnimator& playStack);
            void animateFromLocalPlayerTemp(const CardAnimation& animation, const CardAnimator& playStack);

        private:
            std::pair<glm::vec3, glm::vec3> getEndPositionAndRotation(const CardAnimator& playStack);   // first parameter of the pair is position, second one the rotation

	};
}