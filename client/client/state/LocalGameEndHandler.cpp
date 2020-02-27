#include "LocalGameEndHandler.h"

namespace card {
	LocalGameEndHandler::LocalGameEndHandler(StateManager& stateManager) :
			stateManager(stateManager) {
	}
	void LocalGameEndHandler::onGameEnd() {
		stateManager.changeState("MainMenuState");
	}
}