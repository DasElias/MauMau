#include "ParticipantsOverviewState.h"
#include "ParticipantsOverviewState.h"
#include "ParticipantsOverviewState.h"
#include <iostream>
#include <shared/utils/MathUtils.h>
#include <egui\input\IOHandler.h>
#include <shared/utils/ThreadUtils.h>

namespace card {
	ParticipantsOverviewState::ParticipantsOverviewState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer) :
			State(stateManager),
			eguiRenderer(eguiRenderer) {
	}

	void ParticipantsOverviewState::updateAndRender(float delta) {
		auto& gameFacade = getStateMananger().getGameFacade();
		auto& room = gameFacade->getRoom();
		if(room.canStartGame() && egui::getInputHandler().isKeyDown(KEY_A)) {
			room.requestGameStart();
		}

		if(gameFacade->isGameRunning()) {
			stateManager.changeState("IngameState");
		}
	}

	void ParticipantsOverviewState::onStateEnter() {
		State::onStateEnter();

		
	}


}