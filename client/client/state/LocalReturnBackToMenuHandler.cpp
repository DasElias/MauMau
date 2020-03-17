#include "LocalReturnBackToMenuHandler.h"
#include <egui/model/popups/PopupInfoBox.h>

namespace card {
	LocalReturnBackToMenuHandler::LocalReturnBackToMenuHandler(StateManager& stateManager) :
			stateManager(stateManager) {
	}
	void LocalReturnBackToMenuHandler::returnBackToMenu(ReturnBackToMenuCause cause) {
		stateManager.changeState("MainMenuState");

		if(cause == ReturnBackToMenuCause::GAME_WAS_ABORTED) {
			egui::PopupInfoBox p("MauMau", "Das Spiel wurde abgebrochen.");
			p.show();
		}
	}
}