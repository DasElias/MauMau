#include "NetworkReturnBackToMenuHandler.h"
#include <egui/model/popups/PopupInfoBox.h>

namespace card {
	NetworkReturnBackToMenuHandler::NetworkReturnBackToMenuHandler(StateManager& stateManager) :
			stateManager(stateManager) {
	}
	void NetworkReturnBackToMenuHandler::returnBackToMenu(ReturnBackToMenuCause cause) {
		stateManager.changeState("ParticipantsOverviewState");

		if(cause == ReturnBackToMenuCause::GAME_WAS_ABORTED) {
			egui::PopupInfoBox p("MauMau", "Das Spiel wurde abgebrochen.");
			p.show();
		}
	}
}