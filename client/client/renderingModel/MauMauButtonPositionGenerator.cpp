#include "MauMauButtonPositionGenerator.h"

namespace card {
	MauMauButtonPositionGenerator::MauMauButtonPositionGenerator(ProjectionMatrix& pm, Viewport& vp) :
			worldToScreenConverter(pm, vp) {
	}
	glm::vec2 MauMauButtonPositionGenerator::getPositionOnScreen() const {
		static glm::vec3 worldPosition = {1.275f, 0.405f, 0.7f};
		return worldToScreenConverter.convertWorldToScreen_percent(worldPosition);
	}
}