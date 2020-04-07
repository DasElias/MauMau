#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../model/CardAnimator.h"
#include "PositionedCard.h"
#include "PositionedCardStack.h"
#include "CardDimensions.h"

namespace card {
	class HandCardStackPositionGenerator {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static float const CARD_OVERLAPPING_FACTOR;

		public:
			static float const Z_INDEX_STEP;

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
			std::vector<PositionedCard> generateMatricies_cardStackX(PositionedCardStack positionedCardStack, CardDimensions cardDimensions, float maxWidthOfCardStack, int selectedCardIndex = -1, float selectedCardAddition = 0);
			std::vector<PositionedCard> generateMatricies_cardStackZ(PositionedCardStack positionedCardStack, CardDimensions cardDimensions, float maxWidthOfCardStack, int selectedCardIndex = -1, float selectedCardAddition = 0);
			glm::vec3 getPositionOfCard_cardStackX(int index, std::size_t cardStackLength, glm::vec3 centerPosition, float maxWidthOfCardStack, float widthOfSingleCard);
			glm::vec3 getPositionOfCard_cardStackZ(int index, std::size_t cardStackLength, glm::vec3 centerPosition, float maxWidthOfCardStack, float widthOfSingleCard);

		private:
			void computeCardStackWidth(std::size_t amountOfCards, float maxWidthOfCardStack, float widthOfSingleCard, float& output_finalWidth, float& output_overlappingFactor);
	};
}