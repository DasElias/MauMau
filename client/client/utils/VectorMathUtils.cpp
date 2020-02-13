#include "VectorMathUtils.h"
#include <shared/utils/MathUtils.h>
#include <algorithm>

namespace card {
	glm::vec3 interpolateLinear(float x, float x1, glm::vec3 y1, float x2, glm::vec3 y2) {
		float vecX = interpolateLinear(x, x1, y1.x, x2, y2.x);
		float vecY = interpolateLinear(x, x1, y1.y, x2, y2.y);
		float vecZ = interpolateLinear(x, x1, y1.z, x2, y2.z);

		return {vecX, vecY, vecZ};
	}

	glm::vec3 clamp(const glm::vec3& val, const glm::vec3& limit1, const glm::vec3& limit2) {
		glm::vec3 returnValue;
		returnValue.x = clamp(val.x, fmin(limit1.x, limit2.x), fmax(limit1.x, limit2.x));
		returnValue.y = clamp(val.y, fmin(limit1.y, limit2.y), fmax(limit1.y, limit2.y));
		returnValue.z = clamp(val.z, fmin(limit1.z, limit2.z), fmax(limit1.z, limit2.z));
		return returnValue;
	}
}