#pragma once
#include "../model/CardAnimator.h"
#include <glm/glm.hpp>

namespace card {
	class PositionedCardStack {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::reference_wrapper<const CardAnimator> cardAnimator;
            glm::vec3 centerPosition;
            glm::vec3 rotation;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            PositionedCardStack(const CardAnimator& ca, glm::vec3 centerPosition, glm::vec3 rotation);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            const CardAnimator& getCardAnimator() const;
            glm::vec3 getCenterPosition() const;
            glm::vec3 getRotation() const;
	};
}