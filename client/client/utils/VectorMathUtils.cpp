#include "VectorMathUtils.h"
#include <shared/utils/MathUtils.h>

namespace card {
	glm::vec3 interpolateLinear(float x, float x1, glm::vec3 y1, float x2, glm::vec3 y2) {
		float vecX = interpolateLinear(x, x1, y1.x, x2, y2.x);
		float vecY = interpolateLinear(x, x1, y1.y, x2, y2.y);
		float vecZ = interpolateLinear(x, x1, y1.z, x2, y2.z);

		return {vecX, vecY, vecZ};
	}
}