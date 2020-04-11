#include "DrawnCardPositionComputer.h"
#include <shared/model/HandCardStack.h>
#include <iostream>

namespace card {
	DrawnCardPositionComputer::DrawnCardPositionComputer(CardDimensions dimensions) :
			cardDimensions(dimensions) {
	}
	glm::vec3 DrawnCardPositionComputer::computePosition_x(const CardAnimator& handCardStack, Card cardToDraw, glm::vec3 centerPosition, float maxWidthOfCardStack) {
		int prospectiveIndex = getProspectiveIndex(handCardStack, cardToDraw);
		std::size_t prospectiveSize = getProspectiveSize(handCardStack);
		return handCardStackPositionGenerator.getPositionOfCard_cardStackX(prospectiveIndex, prospectiveSize, centerPosition + glm::vec3(0, cardDimensions.cardHeight, 0), maxWidthOfCardStack, cardDimensions.cardWidth);
	}
	glm::vec3 DrawnCardPositionComputer::computePosition_z(const CardAnimator& handCardStack, Card cardToDraw, glm::vec3 centerPosition, float maxWidthOfCardStack) {
		int prospectiveIndex = getProspectiveIndex(handCardStack, cardToDraw);
		std::size_t prospectiveSize = getProspectiveSize(handCardStack);
		return handCardStackPositionGenerator.getPositionOfCard_cardStackZ(prospectiveIndex, prospectiveSize, centerPosition, maxWidthOfCardStack, cardDimensions.cardWidth);
	}
	int DrawnCardPositionComputer::getProspectiveIndex(const CardAnimator& handCardStack, Card cardToDraw) {
		std::vector<Card> cards = handCardStack.getCards();
	//	cards.insert(cards.begin(), cardToDraw);
		cards.push_back(cardToDraw);

		HandCardStack tempCardStack(cards);
		int prospectiveIndex = tempCardStack.find(cardToDraw);
		std::cout << prospectiveIndex << std::endl;
		return prospectiveIndex;
	}
	std::size_t DrawnCardPositionComputer::getProspectiveSize(const CardAnimator& handCardStack) {
		return handCardStack.getSize() + 1;
	}
}