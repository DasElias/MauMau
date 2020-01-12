#include "JoinRoomState.h"
#include <egui/model/popups/PopupErrorBox.h>
#include <shared/utils/TimeUtils.h>

namespace card {
	JoinRoomState::JoinRoomState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer, NetworkErrorHandler& networkErrorHandler) :
			State(stateManager),
			eguiRenderer(eguiRenderer),
			networkErrorHandler(networkErrorHandler) {
	}
	void JoinRoomState::updateAndRender(float delta) {
		handleResponseIfAvailable();
	}
	void JoinRoomState::onStateEnter() {
		State::onStateEnter();
		createdGameFacade.reset();

		sendRequest("JoinUser" + std::to_string(getMilliseconds()), 1, 0);
	}
	void JoinRoomState::onStateExit() {
		State::onStateExit();
	}
	void JoinRoomState::sendRequest(std::string username, Avatar avatar, RoomCode roomCode) {
		wasResponseReceived = false;

		this->createdGameFacade = std::make_shared<JoinRoomNetworkGameFacade>(networkErrorHandler, username, avatar, roomCode);
		stateManager.setGameFacade(createdGameFacade);
	}
	void JoinRoomState::handleResponseIfAvailable() {		
		if(!wasResponseReceived && createdGameFacade && !createdGameFacade->isWaitingForResponse()) {
			this->wasResponseReceived = true;

			if(createdGameFacade->hasErrorOccuredOnEstablishingConnection()) {
				std::string errorMsg = *(createdGameFacade->getErrorMsgOnEstablishingConnectionInPlainText());
				egui::PopupErrorBox errBox("Verbindung fehlgeschlagen", errorMsg);
				errBox.show();

				createdGameFacade.reset();
			} else {
				stateManager.setGameFacade(createdGameFacade);
				createdGameFacade.reset();

				stateManager.changeState("ParticipantsOverviewState");
			}
		}
	}
}