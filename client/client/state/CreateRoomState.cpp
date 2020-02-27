#include "CreateRoomState.h"
#include <egui/model/popups/PopupErrorBox.h>
#include "RoomLeaveHandlerImpl.h"
#include "NetworkGameEndHandler.h"


namespace card {
	CreateRoomState::CreateRoomState(StateManager& stateManager, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer, NetworkErrorHandler& networkErrorHandler) :
			State(stateManager),
			eguiRenderer(eguiRenderer),
			networkErrorHandler(networkErrorHandler),
			element(std::make_shared<CreateOnlineRoomElement>(avatarTextures, 20)),
			scene(element) {

		element->addBackBtnEventHandler({[this, &stateManager](egui::ActionEvent&) {
			stateManager.changeState("MainMenuState");
		}});
		element->addContinueBtnEventHandler({[this, &stateManager](egui::ActionEvent&) {
			std::optional<std::string> errorMsg = getLocalVerificationErrorMessage();
			if(errorMsg.has_value()) {
				egui::PopupErrorBox eb("Online-Raum erstellen", *errorMsg);
				eb.show();
			} else {
				element->lockInput(true);

				// we have verified the input before
				std::string username = element->getUsernameInput();
				Avatar avatar = element->getSelectedAvatar();
				RoomOptions options = element->getOptions();

				sendRequest(username, avatar, options);
			}
		}});
	}
	void CreateRoomState::updateAndRender(float delta) {
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();

		handleResponseIfAvailable();
	}
	void CreateRoomState::onStateEnter() {
		State::onStateEnter();
		createdGameFacade.reset();
		scene.discardMouseEvents();
	}
	
	void CreateRoomState::sendRequest(std::string username, Avatar avatar, RoomOptions options) {
		this->createdGameFacade = std::make_shared<CreateRoomNetworkGameFacade>(networkErrorHandler, std::make_unique<RoomLeaveHandlerImpl>(stateManager), std::make_unique<NetworkGameEndHandler>(stateManager), username, avatar, options);
		stateManager.setGameFacade(createdGameFacade);
	}
	void CreateRoomState::handleResponseIfAvailable() {		
		if(createdGameFacade && !createdGameFacade->isWaitingForResponse()) {
			element->lockInput(false);

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
	std::optional<std::string> CreateRoomState::getLocalVerificationErrorMessage() {
		std::string usernameInput = element->getUsernameInput();

		if(usernameInput.size() > USERNAME_MAX_LENGTH) return "Dein Nutzername darf maximal " + std::to_string(USERNAME_MAX_LENGTH) + " Zeichen lang sein.";
		if(usernameInput.size() == 0) return "Dein Nutzername darf nicht nicht leer sein.";

		return std::nullopt;
	}
}