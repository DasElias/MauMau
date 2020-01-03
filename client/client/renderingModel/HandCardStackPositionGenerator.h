#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../model/CardAnimator.h"
#include "PositionedCard.h"

namespace card {
	class HandCardStackPositionGenerator {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static float const CARD_OVERLAPPING_FACTOR;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::vector<PositionedCard> generateMatricies_cardStackX(const CardAnimator& cardStack, glm::vec3 centerPosition, glm::vec3 rotation, float maxWidthOfCardStack, float widthOfSingleCard);
			std::vector<PositionedCard> generateMatricies_cardStackZ(const CardAnimator& cardStack, glm::vec3 centerPosition, glm::vec3 rotation, float maxWidthOfCardStack, float widthOfSingleCard);

		private:
			void computeCardStackWidth(std::size_t amountOfCards, float maxWidthOfCardStack, float widthOfSingleCard, float& output_finalWidth, float& output_overlappingFactor);
	};
}