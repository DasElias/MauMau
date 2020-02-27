#include "NetworkGameEndHandler.h"

namespace card {
	NetworkGameEndHandler::NetworkGameEndHandler(StateManager& stateManager) :
			stateManager(stateManager) {
	}
	void NetworkGameEndHandler::onGameEnd() {
		stateManager.changeState("ParticipantsOverviewState");
	}
}