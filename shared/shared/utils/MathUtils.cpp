#include "MathUtils.h"
#include <cstdlib>

namespace card {
	float interpolateLinear(float x, float x1, float y1, float x2, float y2) {
		return y1 + ((y2 - y1) / (x2 - x1)) * (x - x1);
	}


	float avg(float a, float b) {
		return (a + b) / 2;
	}

	int sgn(float val) {
	    return (float(0) < val) - (val < float(0));
	}

	float toRadiands(float deg) {
		return deg * (PI / 180.0f);
	}

	float toDegrees(float rad) {
		return rad * (180.0f / PI);
	}

	std::random_device& getRandomDevice() {
		static std::random_device rd{};
		return rd;
	}
	std::mt19937& getEngine() {
		static std::mt19937 eng(getRandomDevice()());
		return eng;
	}
}

