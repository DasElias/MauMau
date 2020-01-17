#pragma once

namespace card {
	float computeYFromXForQuadratOnScreen(float x);

	// val is [0, 1]
	// return value is [-1, 1]
	float convertToOpenGLCoordinate(float val);
}