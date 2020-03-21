#include "IngameState.h"
#include <iostream>
#include <shared/utils/MathUtils.h>
#include <egui\input\IOHandler.h>
#include <shared/utils/ThreadUtils.h>
#include "../rendering/CardStackPositions.h"

namespace card {
	int const IngameState::FOV = 50;
	float const IngameState::START_VIEWPORT_Z = 6.3f;

	IngameState::IngameState(StateManager& stateManager, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer) :
			State(stateManager),
			projectionMatrix(45, 1, 100),
			viewport({0, 0.15f, START_VIEWPORT_Z}, {-0.5f, 0, 0}),
			worldToScreenConverter(projectionMatrix, viewport),
			playerLabelPositionGenerator(projectionMatrix, viewport),
			eguiRenderer(eguiRenderer),
			sceneRenderer(projectionMatrix, viewport, avatarTextures, eguiRenderer) {
	}

	void IngameState::updateAndRender(float delta) {
		// first we have to reset the z position of the viewport
		glm::vec3 viewportPosition = viewport.getPosition();
		viewportPosition.z = START_VIEWPORT_Z;
		viewport.setPosition(viewportPosition);
		
		updateViewportY();
		updateViewportZ();
		updateViewportY();

		projectionMatrix.update(FOV);
		sceneRenderer.render(delta);

	}

	void IngameState::updateViewportY() {
		float const TOLERANCE = 0.005f;
		float const DESIRED_MIDDLE_POSITION = 0.95f;
		while(getYOnScreenOfMiddleOfHandCards() > (DESIRED_MIDDLE_POSITION + TOLERANCE)) {
			viewport.addPosition(glm::vec3(0, -TOLERANCE, 0));
		}
		while(getYOnScreenOfMiddleOfHandCards() < (DESIRED_MIDDLE_POSITION - TOLERANCE)) {
			viewport.addPosition(glm::vec3(0, TOLERANCE, 0));
		}
	}

	void IngameState::updateViewportZ() {
		float const SCREEN_TOP = 0.0f;

		glm::vec3 newViewportPosition = viewport.getPosition();
		int iterations = 0;
		while(getTopYOfVisAVisPlayerLabel() < SCREEN_TOP && (iterations++) <= 200) {
			newViewportPosition.z += 0.1f;
			viewport.setPosition(newViewportPosition);
		}
	}

	float IngameState::getYOnScreenOfMiddleOfHandCards() {
		return worldToScreenConverter.convertWorldToScreen_percent(HAND_CARDS_LOCAL_POSITION).y;
	}

	float IngameState::getTopYOfVisAVisPlayerLabel() {
		return playerLabelPositionGenerator.getScreenPosForVisAVisPlayerLabel().y;
	}

	void IngameState::onStateEnter() {
		State::onStateEnter();

		auto& gameFacade = getStateMananger().getGameFacade();
		auto& room = gameFacade->getRoom();
		sceneRenderer.onSceneEnter(room);
	}

	void IngameState::onStateExit() {
		State::onStateExit();

		sceneRenderer.onSceneExit();
	}

	
}