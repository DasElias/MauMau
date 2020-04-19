#pragma once
#include <random>

namespace card {
	float const PI = 3.14159265359f;

	float interpolateLinear(float x, float x1, float y1, float x2, float y2);
	float interpolateLinearAndClamp(float x, float x1, float y1, float x2, float y2);

	// incl. min and max
	template<typename T = int>
	T randomInRange(T min, T max);

	template<typename T = float>
	T randomReal();

	template<typename T = float>
	T randomRealInRange(T min, T max);

	template<typename T = float>
	T clamp(T val, T min, T max);

	template<typename Value = float, typename Weight = float>
	Value weightedAvg(Value a, Weight weightA, Value b, Weight weightB);

	float avg(float a, float b);
	int sgn(float value);
	float toRadiands(float deg);
	float toDegrees(float rad);

	std::mt19937& getEngine();
	std::random_device& getRandomDevice();
}

template<typename T>
T card::randomInRange(T min, T max) {
	auto& engine = getEngine();
	std::uniform_int_distribution<T> distr(min, max);
	return distr(engine);
}

template<typename T>
T card::randomReal() {
	return randomRealInRange<T>(0, 1);
}

template<typename T>
T card::randomRealInRange(T min, T max) {
	auto& engine = getEngine();
	std::uniform_real_distribution<T> distr(min, max);
	return distr(engine);
}

template<typename T>
T card::clamp(T val, T min, T max) {
	if(val < min) return min;
	if(val > max) return max;
	return val;
}

template<typename Value, typename Weight>
Value card::weightedAvg(Value a, Weight weightA, Value b, Weight weightB) {
	return (a * weightA + b * weightB) / (weightA + weightB);
}