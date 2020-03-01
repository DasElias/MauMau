#include "LocalReturnBackToMenuHandler.h"

namespace card {
	LocalReturnBackToMenuHandler::LocalReturnBackToMenuHandler(StateManager& stateManager) :
			stateManager(stateManager) {
	}
	void LocalReturnBackToMenuHandler::returnBackToMenu() {
		stateManager.changeState("MainMenuState");
	}
}