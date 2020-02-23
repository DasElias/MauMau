#include "RoomLeaveHandlerImpl.h"

namespace card {
	RoomLeaveHandlerImpl::RoomLeaveHandlerImpl(StateManager& stateManager) :
			stateManager(stateManager) {
	}
	void RoomLeaveHandlerImpl::operator()() {
		stateManager.setGameFacade(nullptr);
		stateManager.changeState("MainMenuState");
	}
}