#include "HandCardIntersectionChecker.h"
#include "HandCardIntersectionChecker.h"
#include "HandCardIntersectionChecker.h"
#include "HandCardIntersectionChecker.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include <egui/input/IOHandler.h>
#include <iostream>
#include <array>
namespace card {
	HandCardIntersectionChecker::HandCardIntersectionChecker(ProjectionMatrix& projectionMatrix, Viewport& viewport) :
			IntersectionChecker(projectionMatrix, viewport) {
	}
	std::optional<int> HandCardIntersectionChecker::getIndexOfIntersectedCardInX(PositionedCardStack positionedCardStack, CardDimensions cardDimensions, float maxWidthOfCardStack) {
		// if the cursor is outside of the screen, we don't have to consider a possible intersection
		if(isCursorOutsideOfScreen()) return std::nullopt;
		
		auto positionedCards = positionGenerator.generateMatricies_cardStackX(positionedCardStack, cardDimensions, maxWidthOfCardStack);
		float widthOfSingleCard = cardDimensions.cardWidth;
		float heightOfSingleCard = cardDimensions.cardHeight;

		// we need to iterate from back to front, since the cards further back can cover the ones before
		for(int counter = positionedCards.size() - 1; counter >= 0; counter--) {
			auto c = positionedCards[counter];

			std::array<glm::vec4, 4> edgesInLocalSpace = {
				glm::vec4(-widthOfSingleCard / 2, heightOfSingleCard / 2, 0.0f, 1.0f),	// top left
				glm::vec4(-widthOfSingleCard / 2, -heightOfSingleCard / 2, 0.0f, 1.0f),	// bottom left
				glm::vec4(widthOfSingleCard / 2, -heightOfSingleCard / 2, 0.0f, 1.0f),	// bottom right
				glm::vec4(widthOfSingleCard / 2, heightOfSingleCard / 2, 0.0f, 1.0f)	// top right
			};

			std::array<glm::vec2, 4> edgesOnScreen;
			for(int i = 0; i < edgesInLocalSpace.size(); i++) {
				glm::vec4 position = projectionMatrix.getProjectionMatrix() * viewport.getViewMatrix() * c.getModelMatrix() * edgesInLocalSpace[i];
				
				glm::vec2 screenCoords = {position.x / position.w, position.y / position.w};
				screenCoords.x = egui::getDisplayHandler().getWidth() * (screenCoords.x + 1) * 0.5f;
				screenCoords.y = egui::getDisplayHandler().getHeight() * (1 - (screenCoords.y + 1) * 0.5f);
				edgesOnScreen[i] = screenCoords;
			}

			glm::vec2 clickPoint = {egui::getCursorHandler().getCursorX(), egui::getCursorHandler().getCursorY()};
			if(isPointInTriangle(clickPoint, edgesOnScreen[2], edgesOnScreen[1], edgesOnScreen[0]) || isPointInTriangle(clickPoint, edgesOnScreen[0], edgesOnScreen[2], edgesOnScreen[3])) {
				return counter;
			}
		}

		return std::nullopt;
	}

	bool HandCardIntersectionChecker::isCursorOutsideOfScreen() {
		auto& cursorHandler = egui::getCursorHandler();
		auto& displayHandler = egui::getDisplayHandler();
		return cursorHandler.getCursorX() < 0 || cursorHandler.getCursorX() > displayHandler.getWidth() || 
				cursorHandler.getCursorY() < 0 || cursorHandler.getCursorY() > displayHandler.getHeight();
	}

	

}