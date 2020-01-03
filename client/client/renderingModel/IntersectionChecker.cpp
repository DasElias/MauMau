#include "IntersectionChecker.h"

namespace card {
	IntersectionChecker::IntersectionChecker(ProjectionMatrix& projectionMatrix, Viewport& viewport) :
			projectionMatrix(projectionMatrix),
			viewport(viewport) {
	}

	bool IntersectionChecker::isPointInTriangle(glm::vec2 point, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3) {
		float d1, d2, d3;
		bool has_neg, has_pos;

		d1 = sign(point, v1, v2);
		d2 = sign(point, v2, v3);
		d3 = sign(point, v3, v1);

		has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
		has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

		return !(has_neg && has_pos);
	}

	float IntersectionChecker::sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}
}