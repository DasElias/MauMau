#pragma once
#include "CardStackMisalignmentGenerator.h"
#include <vector>
#include <random>

namespace card {
	class MauMauCardStackMisalignmentGenerator : public CardStackMisalignmentGenerator {
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            std::mt19937 gen;
            std::vector<float> randomNumbers;
            std::uniform_real_distribution<float> dis;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            MauMauCardStackMisalignmentGenerator(std::size_t precomputeNIndicies);

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            glm::vec3 computeRotationMisalignment(std::size_t index);

	};
}