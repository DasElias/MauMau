#include "CardInterpolator.h"
#include <shared/utils/TimeUtils.h>
#include <shared/utils/MathUtils.h>
#include "../utils/VectorMathUtils.h"

namespace card {
	CardInterpolator::CardInterpolator(CardRenderer& cardRenderer, ProjectionMatrix& projectionMatrix, Viewport& viewport) :
			cardRenderer(cardRenderer),
			projectionMatrix(projectionMatrix),
			viewport(viewport) {
	}
	void CardInterpolator::interpolateAndRender(const CardAnimation& animation, float zIndex, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 middle1Position, glm::vec3 middle1Rotation, glm::vec3 middle2Position, glm::vec3 middle2Rotation, glm::vec3 endPosition, glm::vec3 endRotation,
												 float period1Ratio, float period2Ratio, float period3Ratio) {
		float ratioSum = period1Ratio + period2Ratio + period3Ratio;
		float TOLERANCE = 0.02f;
		if(ratioSum < 1 - TOLERANCE || ratioSum > 1 + TOLERANCE) {
			throw std::runtime_error("ratio arguments doesn't sum up to 1");
		}

		glm::vec3 interpolatedPosition, interpolatedRotation;

		float x = float(getMilliseconds() - animation.animationStartTime);
		float x1 = 0;
		float x2 = animation.duration * period1Ratio;
		float x3 = x2 + animation.duration * period2Ratio;
		float x4 = x3 + animation.duration * period3Ratio;

		if(x < x2) {
			// part 1 of the animation
			interpolatedPosition = interpolateLinear(x, x1, startPosition, x2, middle1Position);
			interpolatedRotation = interpolateLinear(x, x1, startRotation, x2, middle1Rotation);
		} else if(x < x3) {
			// part 2 of the animation
			interpolatedPosition = interpolateLinear(x, x2, middle1Position, x3, middle2Position);
			interpolatedRotation = interpolateLinear(x, x2, middle1Rotation, x3, middle2Rotation);
		} else {
			// part 3 of the animation
			interpolatedPosition = interpolateLinear(x, x3, middle2Position, x4, endPosition);
			interpolatedRotation = interpolateLinear(x, x3, middle2Rotation, x4, endRotation);
		}

		cardRenderer.renderInNextPass({animation.mutatesTo, interpolatedPosition, interpolatedRotation, zIndex}, projectionMatrix, viewport);
	}

	void CardInterpolator::interpolateAndRender(const CardAnimation& animation, float zIndex, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 middlePosition, glm::vec3 middleRotation, glm::vec3 endPosition, glm::vec3 endRotation, float period1Ratio, float period2Ratio) {
		float ratioSum = period1Ratio + period2Ratio;
		float TOLERANCE = 0.02f;
		if(ratioSum < 1 - TOLERANCE || ratioSum > 1 + TOLERANCE) {
			throw std::runtime_error("ratio arguments doesn't sum up to 1");
		}

		glm::vec3 interpolatedPosition, interpolatedRotation;

		float x = float(getMilliseconds() - animation.animationStartTime);
		float x1 = 0;
		float x2 = animation.duration * period1Ratio;
		float x3 = x2 + animation.duration * period2Ratio;

		if(x < x2) {
			// part 1 of the animation
			interpolatedPosition = interpolateLinear(x, x1, startPosition, x2, middlePosition);
			interpolatedRotation = interpolateLinear(x, x1, startRotation, x2, middleRotation);
		} else {
			// part 2 of the animation
			interpolatedPosition = interpolateLinear(x, x2, middlePosition, x3, endPosition);
			interpolatedRotation = interpolateLinear(x, x2, middleRotation, x3, endRotation);
		}

		cardRenderer.renderInNextPass({animation.mutatesTo, interpolatedPosition, interpolatedRotation, zIndex}, projectionMatrix, viewport);
	}

	void CardInterpolator::interpolateAndRender(const CardAnimation& animation, float zIndex, glm::vec3 startPosition, glm::vec3 startRotation, glm::vec3 endPosition, glm::vec3 endRotation) {
		glm::vec3 interpolatedPosition, interpolatedRotation;

		float x = float(getMilliseconds() - animation.animationStartTime);
		float x1 = 0;
		float x2 = float(animation.duration);

		interpolatedPosition = interpolateLinear(x, x1, startPosition, x2, endPosition);
		interpolatedRotation = interpolateLinear(x, x1, startRotation, x2, endRotation);

		cardRenderer.renderInNextPass({animation.mutatesTo, interpolatedPosition, interpolatedRotation, zIndex}, projectionMatrix, viewport);

	}
}