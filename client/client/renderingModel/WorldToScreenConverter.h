#pragma once
#include "Viewport.h"
#include "ProjectionMatrix.h"

namespace card {
	class WorldToScreenConverter {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            ProjectionMatrix& projectionMatrix;
            Viewport& viewport;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            WorldToScreenConverter(ProjectionMatrix& projectionMatrix, Viewport& viewport);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            glm::vec2 convertWorldToScreen_px(glm::vec3 worldCoords) const;
            glm::vec2 convertWorldToScreen_percent(glm::vec3 worldCoords) const;


	};
}