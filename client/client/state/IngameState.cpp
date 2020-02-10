#include "IngameState.h"
#include "IngameState.h"
#include "IngameState.h"
#include <iostream>
#include <shared/utils/MathUtils.h>
#include <egui\input\IOHandler.h>
#include <shared/utils/ThreadUtils.h>

namespace card {
	IngameState::IngameState(StateManager& stateManager, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer) :
			State(stateManager),
			projectionMatrix(45, 1, 100),
			viewport({0, 0.2f, 7.0f}, {-0.5f, 0, 0}),
			eguiRenderer(eguiRenderer),
			sceneRenderer(projectionMatrix, viewport, avatarTextures, eguiRenderer) {
	}

	std::string vec3ToString(glm::vec3 vec) {
		return "(" + std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z) + ")";
	}

	void IngameState::updateAndRender(float delta) {
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
		if(egui::getInputHandler().isKeyDown(KEY_6)) {
			viewport.setRotation(glm::vec3(-PI / 8, 0, 0));
			std::cout << vec3ToString(viewport.getRotation()) << std::endl;
		}

	//	projectionMatrix.update(45);
		sceneRenderer.render(delta);

	}

	void IngameState::onStateEnter() {
		State::onStateEnter();

		auto& gameFacade = getStateMananger().getGameFacade();
		auto& game = gameFacade->getGame();
		sceneRenderer.onSceneEnter(game);
	}

	void IngameState::onStateExit() {
		State::onStateExit();

		sceneRenderer.onSceneExit();
	}


}