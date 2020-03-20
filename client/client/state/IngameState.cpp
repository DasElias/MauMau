#include "IngameState.h"
#include "IngameState.h"
#include "IngameState.h"
#include <iostream>
#include <shared/utils/MathUtils.h>
#include <egui\input\IOHandler.h>
#include <shared/utils/ThreadUtils.h>
#include "../rendering/CardStackPositions.h"

namespace card {
	IngameState::IngameState(StateManager& stateManager, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer) :
			State(stateManager),
			projectionMatrix(45, 1, 100),
			viewport({0, 0.15f, 6.3f}, {-0.5f, 0, 0}),
			worldToScreenConverter(projectionMatrix, viewport),
			eguiRenderer(eguiRenderer),
			sceneRenderer(projectionMatrix, viewport, avatarTextures, eguiRenderer),
			unixTimeGameHasEnded(GAME_HASNT_ENDED) {
	}

	std::string vec3ToString(glm::vec3 vec) {
		return "(" + std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z) + ")";
	}

	void IngameState::updateAndRender(float delta) {
		// change state if game has ended
		auto& gameFacade = getStateMananger().getGameFacade();
		auto& game = gameFacade->getGame();
		if(game.hasGameEnded() && unixTimeGameHasEnded != GAME_HASNT_ENDED) {
			unixTimeGameHasEnded = getMilliseconds();
		}
		if(unixTimeGameHasEnded + 5000 < getMilliseconds()) {

		}

		static int fov = 50;

		// render game scene
		if(egui::getInputHandler().isKeyDown(KEY_1)) {
			viewport.setPosition(viewport.getPosition() + glm::vec3(0, 0, 0.05f));
			std::cout << vec3ToString(viewport.getPosition()) << std::endl;
		}
		if(egui::getInputHandler().isKeyDown(KEY_2)) {
			viewport.setPosition(viewport.getPosition() - glm::vec3(0, 0, 0.05f));
			std::cout << vec3ToString(viewport.getPosition()) << std::endl;
		}

		if(egui::getInputHandler().isKeyDown(KEY_4)) {
			viewport.setRotation(viewport.getRotation() + glm::vec3(0.01f, 0, 0));
			std::cout << vec3ToString(viewport.getRotation()) << std::endl;
		}
		if(egui::getInputHandler().isKeyDown(KEY_5)) {
			viewport.setRotation(viewport.getRotation() - glm::vec3(0.01f, 0, 0));
			std::cout << vec3ToString(viewport.getRotation()) << std::endl;
		}
		if(egui::getInputHandler().isKeyDown(KEY_7)) {
			fov++;
			std::cout << fov << std::endl;
		}
		if(egui::getInputHandler().isKeyDown(KEY_8)) {
			fov--;
			std::cout << fov << std::endl;
		}

		float const TOLERANCE = 0.005f;
		float const DESIRED_MIDDLE_POSITION = 0.95f;
		while(getYOnScreenOfMiddleOfHandCards() > (DESIRED_MIDDLE_POSITION + TOLERANCE)) {
			viewport.setPosition(viewport.getPosition() - glm::vec3(0, TOLERANCE, 0));
		}
		while(getYOnScreenOfMiddleOfHandCards() < (DESIRED_MIDDLE_POSITION - TOLERANCE)) {
			viewport.setPosition(viewport.getPosition() + glm::vec3(0, TOLERANCE, 0));
		}

		if(egui::getInputHandler().isKeyDown(KEY_9)) {
			std::cout << getYOnScreenOfMiddleOfHandCards() << std::endl;
		}

		projectionMatrix.update(fov);
		sceneRenderer.render(delta);

	}

	float IngameState::getYOnScreenOfMiddleOfHandCards() {
		return worldToScreenConverter.convertWorldToScreen_percent(HAND_CARDS_LOCAL_POSITION).y;
	}

	void IngameState::onStateEnter() {
		State::onStateEnter();

		auto& gameFacade = getStateMananger().getGameFacade();
		auto& room = gameFacade->getRoom();
		sceneRenderer.onSceneEnter(room);
		unixTimeGameHasEnded = GAME_HASNT_ENDED;
	}

	void IngameState::onStateExit() {
		State::onStateExit();

		sceneRenderer.onSceneExit();
	}

	
}