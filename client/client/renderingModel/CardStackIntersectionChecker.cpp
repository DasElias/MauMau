#include "CardStackIntersectionChecker.h"
#include "CardStackIntersectionChecker.h"
#include "CardStackIntersectionChecker.h"
#include <glm/gtc/matrix_transform.hpp>
#include <egui/input/IOHandler.h>
#include <array>
#include <iostream>

namespace card {
	CardStackIntersectionChecker::CardStackIntersectionChecker(ProjectionMatrix& projectionMatrix, Viewport& viewport) :
			IntersectionChecker(projectionMatrix, viewport) {
	}
	bool CardStackIntersectionChecker::doesIntersect(std::size_t cardStackSize, glm::vec3 centerPosition, glm::vec3 rotation, float heightAdditionPerCard, float widthOfSingleCard, float heightOfSingleCard) {
		float height = 1 * heightAdditionPerCard * (cardStackSize - 1);
		std::array<glm::vec4, 4> edgesInLocalSpace = {
			glm::vec4(-widthOfSingleCard / 2, heightOfSingleCard / 2, 0, 1.0f),		// top left
			glm::vec4(-widthOfSingleCard / 2, -heightOfSingleCard / 2, 0, 1.0f),	// bottom left
			glm::vec4(widthOfSingleCard / 2, -heightOfSingleCard / 2, 0, 1.0f),		// bottom right
			glm::vec4(widthOfSingleCard / 2, heightOfSingleCard / 2, 0, 1.0f),		// top right
		};

		std::array<glm::vec2, 4> bottomEdgesOnScreen = generateScreenPositions(edgesInLocalSpace, centerPosition, rotation);
		std::array<glm::vec2, 4> topEdgesOnScreen = generateScreenPositions(edgesInLocalSpace, centerPosition + glm::vec3(0, height, 0), rotation);

		
		glm::vec2 clickPoint = {egui::getCursorHandler().getCursorX(), egui::getCursorHandler().getCursorY()};
		return isPointInTriangle(clickPoint, topEdgesOnScreen[0], topEdgesOnScreen[1], topEdgesOnScreen[2]) ||	// TOP
				isPointInTriangle(clickPoint, topEdgesOnScreen[0], topEdgesOnScreen[2], topEdgesOnScreen[3]) ||	// TOP
				isPointInTriangle(clickPoint, topEdgesOnScreen[0], topEdgesOnScreen[3], bottomEdgesOnScreen[0]) ||	// FRONT
				isPointInTriangle(clickPoint, topEdgesOnScreen[3], bottomEdgesOnScreen[0], bottomEdgesOnScreen[3]);	// FRONT
	}
	std::array<glm::vec2, 4> CardStackIntersectionChecker::generateScreenPositions(std::array<glm::vec4, 4>& positionsInLocalSpace, glm::vec3 centerPosition, glm::vec3 rotation) {
		glm::mat4 modelMatrix(1.0);
		modelMatrix = glm::translate(modelMatrix, centerPosition);
		modelMatrix = glm::rotate(modelMatrix, rotation.x, {1, 0, 0});
		modelMatrix = glm::rotate(modelMatrix, rotation.y, {0, 1, 0});
		modelMatrix = glm::rotate(modelMatrix, rotation.z, {0, 0, 1});

		std::array<glm::vec2, 4> positionsOnScreen;
		for(std::size_t i = 0; i < 4; i++) {
			glm::vec4 position = projectionMatrix.getProjectionMatrix() * viewport.getViewMatrix() * modelMatrix * positionsInLocalSpace[i];

			glm::vec2 screenCoords = {position.x / position.w, position.y / position.w};
			screenCoords.x = egui::getDisplayHandler().getWidth() * (screenCoords.x + 1) * 0.5f;
			screenCoords.y = egui::getDisplayHandler().getHeight() * (1 - (screenCoords.y + 1) * 0.5f);
			positionsOnScreen[i] = screenCoords;
		}
		return positionsOnScreen;
	}
}