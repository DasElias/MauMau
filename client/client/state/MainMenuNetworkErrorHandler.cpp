#include "MainMenuNetworkErrorHandler.h"
#include <egui/model/popups/PopupErrorBox.h>

namespace card {
	MainMenuNetworkErrorHandler::MainMenuNetworkErrorHandler(StateManager& stateManager) :
			stateManager(stateManager) {
	}
	void MainMenuNetworkErrorHandler::resetAndShowError(std::string errorMessage, std::string title) {
		egui::PopupErrorBox errorBox(title, errorMessage);
		errorBox.show();

//		stateManager.changeState("MainMenuState");

	}
}