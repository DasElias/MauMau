#include "CreateRoomState.h"
#include <egui/model/popups/PopupErrorBox.h>

namespace card {
	CreateRoomState::CreateRoomState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer, NetworkErrorHandler& networkErrorHandler) :
			State(stateManager),
			eguiRenderer(eguiRenderer),
			networkErrorHandler(networkErrorHandler) {
	}
	void CreateRoomState::updateAndRender(float delta) {
		handleResponseIfAvailable();
	}
	void CreateRoomState::onStateEnter() {
		State::onStateEnter();
		createdGameFacade.reset();

		sendRequest("CreateUser", 1);
	}
	void CreateRoomState::onStateExit() {
		State::onStateExit();
	}
	void CreateRoomState::sendRequest(std::string username, Avatar avatar) {
		wasResponseReceived = false;

		this->createdGameFacade = std::make_shared<CreateRoomNetworkGameFacade>(networkErrorHandler, username, avatar);
		stateManager.setGameFacade(createdGameFacade);
	}
	void CreateRoomState::handleResponseIfAvailable() {		
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