#include "IngameState.h"
#include "IngameState.h"
#include "IngameState.h"
#include <iostream>
#include <shared/utils/MathUtils.h>
#include <egui\input\IOHandler.h>
#include <shared/utils/ThreadUtils.h>

namespace card {
	IngameState::IngameState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer) :
			State(stateManager),
			projectionMatrix(45, 1, 100),
			viewport({0, 0, 10}, {-PI / 8, 0, 0}),
			eguiRenderer(eguiRenderer),
			sceneRenderer(projectionMatrix, viewport, eguiRenderer) {
	}

	void IngameState::updateAndRender(float delta) {
		// render game scene
		if(egui::getInputHandler().isKeyDown(KEY_1)) {
			viewport.setPosition(viewport.getPosition() + glm::vec3(0, 0, 0.05f));
		}
		if(egui::getInputHandler().isKeyDown(KEY_2)) {
			viewport.setPosition(viewport.getPosition() - glm::vec3(0, 0, 0.05f));
		}
		if(egui::getInputHandler().isKeyDown(KEY_3)) {
			viewport.setPosition(glm::vec3(0, 0, 5));
		}

		if(egui::getInputHandler().isKeyDown(KEY_4)) {
			viewport.setRotation(viewport.getRotation() + glm::vec3(0.01f, 0, 0));
		}
		if(egui::getInputHandler().isKeyDown(KEY_5)) {
			viewport.setRotation(viewport.getRotation() - glm::vec3(0.01f, 0, 0));
		}
		if(egui::getInputHandler().isKeyDown(KEY_6)) {
			viewport.setRotation(glm::vec3(-PI / 8, 0, 0));
		}

/*		static std::function<void(void)> addFct = [this]() {
			auto& game = getStateMananger().getGameFacade()->getGame();
			game.getOpponents()[2]->playCardLocal({1}, game.playCardStack, false);

			threadUtils_invokeIn(5000, addFct);
		};
		static bool wasSet = false;
		if(! wasSet) {
			addFct();
			wasSet = true;
		}*/

		std::shared_ptr<AbstractGameFacade> gameFacade = getStateMananger().getGameFacade();


		sceneRenderer.setGame(gameFacade->getGame());
		sceneRenderer.render();

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