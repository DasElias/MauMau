#pragma once
#include <glm/vec3.hpp>

namespace card {
	glm::vec3 interpolateLinear(float x, float x1, glm::vec3 y1, float x2, glm::vec3 y2);
	glm::vec3 clamp(const glm::vec3& val, const glm::vec3& min, const glm::vec3& max);
	glm::vec3 avg(const glm::vec3& a, const glm::vec3& b);
	glm::vec3 weightedAvg(const glm::vec3& a, float aWeight, const glm::vec3& b, float bWeight);
}