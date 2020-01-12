#include "CreateLocalRoomState.h"
#include <egui/model/popups/PopupErrorBox.h>

namespace card {
	CreateLocalRoomState::CreateLocalRoomState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer) :
			State(stateManager),
			eguiRenderer(eguiRenderer) {
	}
	void CreateLocalRoomState::updateAndRender(float delta) {
		auto gameFacade = std::make_shared<LocalGameFacade>("Lokaler Spieler", 1);
		stateManager.setGameFacade(gameFacade);
		gameFacade->getRoom().requestGameStart();

		stateManager.changeState("IngameState");
	}
	void CreateLocalRoomState::onStateEnter() {
		State::onStateEnter();

		
	}
	void CreateLocalRoomState::onStateExit() {
		State::onStateExit();
	}
}