#include "CardStackPositions.h"
#include "CardRenderer.h"
#include <shared/utils/MathUtils.h>

namespace card {
	const glm::vec3 DRAW_CARDS_POSITION = {-0.5, 0, 0};
	const glm::vec3 PLAY_CARDS_POSITION = {0.5, 0, 0};
	const glm::vec3 HAND_CARDS_LOCAL_POSITION = {0, -CardRenderer::HEIGHT / 2, 1.25};
	const glm::vec3 HAND_CARDS_OPPONENT_VISAVIS_POSITION = {0, CardRenderer::HEIGHT / 2, -1.25};
	const glm::vec3 HAND_CARDS_OPPONENT_RIGHT_POSITION = {2, CardRenderer::HEIGHT / 2, 0};
	const glm::vec3 HAND_CARDS_OPPONENT_LEFT_POSITION = {-2, CardRenderer::HEIGHT / 2, 0};

	const glm::vec3 DRAW_CARDS_ROTATION = {PI / 2, 0, 0};
	const glm::vec3 PLAY_CARDS_ROTATION = {PI / 2, PI, 0};
	const glm::vec3 HAND_CARDS_LOCAL_ROTATION = {0, 0, 0};
	const glm::vec3 HAND_CARDS_OPPONENT_VISAVIS_ROTATION = {PI, 0, 0};
	const glm::vec3 HAND_CARDS_OPPONENT_RIGHT_ROTATION = {PI, PI / 2, 0};
	const glm::vec3 HAND_CARDS_OPPONENT_LEFT_ROTATION = {PI, -PI / 2, 0};

	const float LEFT_RIGHT_OPPONENT_CARDS_WIDTH = 2;
	const float FRONT_BACK_OPPONENT_CARDS_WIDTH = 4;
}