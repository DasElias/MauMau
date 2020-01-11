#pragma once
#include <cstddef>
#include <glm/glm.hpp>

namespace card {
	class CardStackMisalignmentGenerator {
        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            virtual ~CardStackMisalignmentGenerator() = default;

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            virtual glm::vec3 computeRotationMisalignment(std::size_t index) =0;

	};
}