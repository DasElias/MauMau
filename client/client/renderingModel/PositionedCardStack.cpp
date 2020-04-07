#include "PositionedCardStack.h"

namespace card {
	PositionedCardStack::PositionedCardStack(const CardAnimator& ca, glm::vec3 centerPosition, glm::vec3 rotation, float startZIndex) :
			cardAnimator(ca),
			centerPosition(centerPosition),
			rotation(rotation),
			startZIndex(startZIndex) {
	}
	const CardAnimator& PositionedCardStack::getCardAnimator() const {
		return cardAnimator;
	}
	glm::vec3 PositionedCardStack::getCenterPosition() const {
		return centerPosition;
	}
	glm::vec3 PositionedCardStack::getRotation() const {
		return rotation;
	}
	float PositionedCardStack::getStartZIndex() const {
		return startZIndex;
	}
}