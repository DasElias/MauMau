#pragma once
#include <random>

namespace card {
	float const PI = 3.14159265359f;

	float interpolateLinear(float x, float x1, float y1, float x2, float y2);

	// incl. min and max
	template<typename T = int>
	T randomInRange(T min, T max);

	float avg(float a, float b);
	int sgn(float value);
	float toRadiands(float deg);
	float toDegrees(float rad);

	std::random_device& getRandomDevice();
}

template<typename T>
T card::randomInRange(T min, T max) {
	static std::mt19937 eng(getRandomDevice()());
	std::uniform_int_distribution<T> distr(min, max);
	return distr(eng);
}