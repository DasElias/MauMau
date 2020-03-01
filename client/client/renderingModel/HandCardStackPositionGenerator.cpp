#include "HandCardStackPositionGenerator.h"

#include <glm/gtc/matrix_transform.hpp>


namespace card {
	float const HandCardStackPositionGenerator::CARD_OVERLAPPING_FACTOR = 0.5f;

	std::vector<PositionedCard> HandCardStackPositionGenerator::generateMatricies_cardStackX(const CardAnimator& cardStack, glm::vec3 centerPosition, glm::vec3 rotation, float maxWidthOfCardStack, float widthOfSingleCard) {
		float computedWidth = 0;
		float computedOverlappingFactor = 0;
		computeCardStackWidth(cardStack.getSize(), maxWidthOfCardStack, widthOfSingleCard, computedWidth, computedOverlappingFactor);

		float cardCenterX = centerPosition.x - (computedWidth / 2) + widthOfSingleCard / 2;

		glm::mat4 modelMatrix(1.0);
		modelMatrix = glm::rotate(modelMatrix, rotation.x, {1, 0, 0});
		modelMatrix = glm::rotate(modelMatrix, rotation.y, {0, 1, 0});
		modelMatrix = glm::rotate(modelMatrix, rotation.z, {0, 0, 1});

		std::vector<PositionedCard> cards;
		for(auto& c : cardStack) {
			glm::vec3 singleCardPosition(cardCenterX, centerPosition.y, centerPosition.z);
			cards.push_back({c, singleCardPosition, rotation});

			cardCenterX += widthOfSingleCard * (1 - computedOverlappingFactor);
		}

		return cards;
	}
	std::vector<PositionedCard> HandCardStackPositionGenerator::generateMatricies_cardStackZ(const CardAnimator& cardStack, glm::vec3 centerPosition, glm::vec3 rotation, float maxWidthOfCardStack, float widthOfSingleCard) {
		float computedWidth = 0;
		float computedOverlappingFactor = 0;
		computeCardStackWidth(cardStack.getSize(), maxWidthOfCardStack, widthOfSingleCard, computedWidth, computedOverlappingFactor);

		float cardCenterZ = centerPosition.z - (computedWidth / 2) + widthOfSingleCard / 2;

		glm::mat4 modelMatrix(1.0);
		modelMatrix = glm::rotate(modelMatrix, rotation.x, {1, 0, 0});
		modelMatrix = glm::rotate(modelMatrix, rotation.y, {0, 1, 0});
		modelMatrix = glm::rotate(modelMatrix, rotation.z, {0, 0, 1});

		std::vector<PositionedCard> cards;
		for(auto& c : cardStack) {
			glm::vec3 singleCardPosition(centerPosition.x, centerPosition.y, cardCenterZ);
			cards.push_back({c, singleCardPosition, rotation});

			cardCenterZ += widthOfSingleCard * (1 - computedOverlappingFactor);
		}

		return cards;
	}

	glm::vec3 HandCardStackPositionGenerator::getPositionOfCard_cardStackX(int index, std::size_t cardStackLength, glm::vec3 centerPosition, float maxWidthOfCardStack, float widthOfSingleCard) {
		if(cardStackLength == 0) return centerPosition;

		float computedWidth = 0;
		float computedOverlappingFactor = 0;
		computeCardStackWidth(cardStackLength, maxWidthOfCardStack, widthOfSingleCard, computedWidth, computedOverlappingFactor);
		float cardCenterX = centerPosition.x - (computedWidth / 2) + widthOfSingleCard / 2;
		cardCenterX += index * widthOfSingleCard * (1 - computedOverlappingFactor);
		return {cardCenterX, centerPosition.y, centerPosition.z};
	}

	glm::vec3 HandCardStackPositionGenerator::getPositionOfCard_cardStackZ(int index, std::size_t cardStackLength, glm::vec3 centerPosition, float maxWidthOfCardStack, float widthOfSingleCard) {
		if(cardStackLength == 0) return centerPosition;

		float computedWidth = 0;
		float computedOverlappingFactor = 0;
		computeCardStackWidth(cardStackLength, maxWidthOfCardStack, widthOfSingleCard, computedWidth, computedOverlappingFactor);
		float cardCenterZ = centerPosition.z - (computedWidth / 2) + widthOfSingleCard / 2;
		cardCenterZ += index * widthOfSingleCard * (1 - computedOverlappingFactor);
		return {centerPosition.x, centerPosition.y, cardCenterZ};
	}

	void HandCardStackPositionGenerator::computeCardStackWidth(std::size_t amountOfCards, float maxWidthOfCardStack, float widthOfSingleCard, float& output_finalWidth, float& output_overlappingFactor) {
		output_finalWidth = 0;
		output_overlappingFactor = CARD_OVERLAPPING_FACTOR;

		do {
			output_finalWidth = widthOfSingleCard * (amountOfCards - output_overlappingFactor * (amountOfCards - 1));

			if(output_finalWidth <= maxWidthOfCardStack) break;
			else output_overlappingFactor += 0.005f;

		} while(true);

	}
} 