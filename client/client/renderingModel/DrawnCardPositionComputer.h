#pragma once
#include "HandCardStackPositionGenerator.h"

namespace card {
	class DrawnCardPositionComputer {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            HandCardStackPositionGenerator handCardStackPositionGenerator;
            CardDimensions cardDimensions;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            DrawnCardPositionComputer(CardDimensions dimensions);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            glm::vec3 computePosition_x(const CardAnimator& handCardStack, Card cardToDraw, glm::vec3 centerPosition, float maxWidthOfCardStack);
            glm::vec3 computePosition_z(const CardAnimator& handCardStack, Card cardToDraw, glm::vec3 centerPosition, float maxWidthOfCardStack);

        private:    
            int getProspectiveIndex(const CardAnimator& handCardStack, Card cardToDraw);
            std::size_t getProspectiveSize(const CardAnimator& handCardStack);
	};
}