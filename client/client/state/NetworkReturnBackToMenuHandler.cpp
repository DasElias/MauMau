#include "NetworkReturnBackToMenuHandler.h"

namespace card {
	NetworkReturnBackToMenuHandler::NetworkReturnBackToMenuHandler(StateManager& stateManager) :
			stateManager(stateManager) {
	}
	void NetworkReturnBackToMenuHandler::returnBackToMenu() {
		stateManager.changeState("ParticipantsOverviewState");
	}
}