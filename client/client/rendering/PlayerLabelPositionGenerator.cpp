#include "PlayerLabelPositionGenerator.h"
#include "CardStackPositions.h"
#include "CardRenderer.h"
#include "../gui/PlayerLabel.h"

namespace card {
	int const PlayerLabelPositionGenerator::SPACE_BETWEEN_LABEL_AND_CARDS_LOCAL_VISAVIS_PX = 5;
	float const PlayerLabelPositionGenerator::LEFT_RIGHT_TRANSLATION = 0.045f;

	PlayerLabelPositionGenerator::PlayerLabelPositionGenerator(ProjectionMatrix& pm, Viewport& v) :
			converter(pm, v) {
	}
	glm::vec2 PlayerLabelPositionGenerator::getScreenPosForLocalPlayerLabel() const {
		glm::vec3 positionWorld = HAND_CARDS_LOCAL_POSITION + glm::vec3(0, CardRenderer::HEIGHT * 0.5f, 0);
		glm::vec2 positionScreen = converter.convertWorldToScreen_percent(positionWorld);
		positionScreen.x = 0.25f;
		positionScreen.y -= PlayerLabel::getHeightInPercent();
		positionScreen.y -= egui::y_pixelToPercent(SPACE_BETWEEN_LABEL_AND_CARDS_LOCAL_VISAVIS_PX);
		return positionScreen;
	}
	glm::vec2 PlayerLabelPositionGenerator::getScreenPosForVisAVisPlayerLabel() const {
		glm::vec3 positionWorld = HAND_CARDS_OPPONENT_VISAVIS_POSITION + glm::vec3(0, CardRenderer::HEIGHT * 0.5f, 0);
		glm::vec2 positionScreen = converter.convertWorldToScreen_percent(positionWorld);
		positionScreen.x -= PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN / 2;
		positionScreen.y -= PlayerLabel::getHeightInPercent();
		positionScreen.y -= egui::y_pixelToPercent(SPACE_BETWEEN_LABEL_AND_CARDS_LOCAL_VISAVIS_PX);
		return positionScreen;
	}
	glm::vec2 PlayerLabelPositionGenerator::getScreenPosForLeftPlayerLabel() const {
		glm::vec3 positionWorld = HAND_CARDS_OPPONENT_LEFT_POSITION + glm::vec3(0, CardRenderer::HEIGHT * 0.5f, 0);
		glm::vec2 positionScreen = converter.convertWorldToScreen_percent(positionWorld);
		positionScreen.x -= PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN / 2;
		positionScreen.x -= LEFT_RIGHT_TRANSLATION;
		positionScreen.y -= PlayerLabel::getHeightInPercent();
		return positionScreen;
	}
	glm::vec2 PlayerLabelPositionGenerator::getScreenPosForRightPlayerLabel() const {
		glm::vec3 positionWorld = HAND_CARDS_OPPONENT_RIGHT_POSITION + glm::vec3(0, CardRenderer::HEIGHT * 0.5f, 0);
		glm::vec2 positionScreen = converter.convertWorldToScreen_percent(positionWorld);
		positionScreen.x -= PlayerLabel::IMAGE_WIDTH_RELATIVE_ON_SCREEN / 2;
		positionScreen.x += LEFT_RIGHT_TRANSLATION;
		positionScreen.y -= PlayerLabel::getHeightInPercent();
		return positionScreen;
	}
}