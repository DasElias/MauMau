#include "DimensionsConversionUtils.h"
#include <egui/input/IOHandler.h>

namespace card {
	float card::computeYFromXForQuadratOnScreen(float x) {
		return (x / egui::getDisplayHandler().getHeight()) * egui::getDisplayHandler().getWidth();
	}
	float convertToOpenGLCoordinate(float val) {
		return (val * 2) - 1;
	}
}