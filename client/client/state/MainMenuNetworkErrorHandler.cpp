#include "MainMenuNetworkErrorHandler.h"
#include "MainMenuNetworkErrorHandler.h"
#include <egui/model/popups/PopupErrorBox.h>
#include <egui/model/popups/PopupInfoBox.h>

namespace card {
	MainMenuNetworkErrorHandler::MainMenuNetworkErrorHandler(StateManager& stateManager) :
			stateManager(stateManager) {
	}
	void MainMenuNetworkErrorHandler::resetAndShowError(std::string errorMessage, std::string title) {
		egui::PopupErrorBox errorBox(title, errorMessage);
		errorBox.show();

		stateManager.changeState("MainMenuState");
		stateManager.setGameFacade(nullptr);

	}
	void MainMenuNetworkErrorHandler::onKick() {
		stateManager.changeState("MainMenuState");
		egui::PopupInfoBox infoBox("MauMau", "Du wurdest dem Online-Raum verwiesen!");
		infoBox.show();
		stateManager.setGameFacade(nullptr);
	}
}