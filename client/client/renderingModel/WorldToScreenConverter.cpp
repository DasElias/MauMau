#include "WorldToScreenConverter.h"
#include <egui/input/IOHandler.h>

namespace card {
	WorldToScreenConverter::WorldToScreenConverter(ProjectionMatrix& projectionMatrix, Viewport& viewport) :
			projectionMatrix(projectionMatrix),
			viewport(viewport) {
	}
	glm::vec2 WorldToScreenConverter::convertWorldToScreen_px(glm::vec3 worldCoords) const {
		glm::vec2 screenInPercent = convertWorldToScreen_percent(worldCoords);
		screenInPercent.x *= egui::getDisplayHandler().getWidth();
		screenInPercent.y *= egui::getDisplayHandler().getHeight();
		return screenInPercent;
	}
	glm::vec2 WorldToScreenConverter::convertWorldToScreen_percent(glm::vec3 worldCoords_vec3) const {
		glm::vec4 worldCoords(worldCoords_vec3.x, worldCoords_vec3.y, worldCoords_vec3.z, 1.0f);
		glm::vec4 position = projectionMatrix.getProjectionMatrix() * viewport.getViewMatrix() * worldCoords;
		glm::vec2 screenCoords = {position.x / position.w, position.y / position.w};
		screenCoords.x = (screenCoords.x + 1) * 0.5f;
		screenCoords.y = (1 - (screenCoords.y + 1) * 0.5f);
		return screenCoords;
	}
}