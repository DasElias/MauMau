#pragma once
#include "../renderingModel/WorldToScreenConverter.h"

namespace card {
	class PlayerLabelPositionGenerator {
        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static int const SPACE_BETWEEN_LABEL_AND_CARDS_LOCAL_PX;
            static int const SPACE_BETWEEN_LABEL_AND_CARDS_VISAVIS_PX;
            static float const LEFT_RIGHT_TRANSLATION;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            WorldToScreenConverter converter;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            PlayerLabelPositionGenerator(ProjectionMatrix& pm, Viewport& v);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            glm::vec2 getScreenPosForLocalPlayerLabel() const;
            glm::vec2 getScreenPosForVisAVisPlayerLabel() const;
            glm::vec2 getScreenPosForLeftPlayerLabel() const;
            glm::vec2 getScreenPosForRightPlayerLabel() const;
	};
}