#include "PositionedCardStack.h"

namespace card {
	PositionedCardStack::PositionedCardStack(const CardAnimator& ca, glm::vec3 centerPosition, glm::vec3 rotation) :
			cardAnimator(ca),
			centerPosition(centerPosition),
			rotation(rotation) {
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
}