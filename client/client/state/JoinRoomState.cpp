#include "JoinRoomState.h"
#include "JoinRoomState.h"
#include "JoinRoomState.h"
#include <egui/model/popups/PopupErrorBox.h>
#include <shared/utils/TimeUtils.h>
#include "RoomLeaveHandlerImpl.h"
#include "NetworkReturnBackToMenuHandler.h"

namespace card {
	JoinRoomState::JoinRoomState(StateManager& stateManager, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer, NetworkErrorHandler& networkErrorHandler) :
			State(stateManager),
			eguiRenderer(eguiRenderer),
			networkErrorHandler(networkErrorHandler),
			element(std::make_shared<JoinOnlineRoomElement>(avatarTextures, USERNAME_MAX_LENGTH, ROOM_CODE_LENGTH)),
			scene(element) {

		element->addBackBtnEventHandler({[this, &stateManager](egui::ActionEvent&) {
			stateManager.changeState("MainMenuState");
		}});
		element->addContinueBtnEventHandler({[this, &stateManager](egui::ActionEvent&) {
			std::optional<std::string> errorMsg = getLocalVerificationErrorMessage();
			if(errorMsg.has_value()) {
				egui::PopupErrorBox eb("Online-Raum beitreten", *errorMsg);
				eb.show();
			} else {
				element->lockInput(true);

				// we have verified the input before
				std::string username = element->getUsernameInput();
				RoomCode roomCode = std::stoul(element->getRoomCodeInput());
				Avatar avatar = element->getSelectedAvatar();

				sendRequest(username, avatar, roomCode);
			}
		}});
	}
	void JoinRoomState::updateAndRender(float delta) {
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();

		handleResponseIfAvailable();
	}
	void JoinRoomState::onStateEnter() {
		State::onStateEnter();
		scene.discardMouseEvents();
	}
	std::optional<std::string> JoinRoomState::getLocalVerificationErrorMessage() {
		std::string usernameInput = element->getUsernameInput();
		std::string roomCodeInput = element->getRoomCodeInput();

		if(usernameInput.size() > USERNAME_MAX_LENGTH) return "Dein Nutzername darf maximal " + std::to_string(USERNAME_MAX_LENGTH) + " Zeichen lang sein.";
		if(usernameInput.size() == 0) return "Dein Nutzername darf nicht nicht leer sein.";
		if(roomCodeInput.size() != ROOM_CODE_LENGTH) return "Bitte gib einen dreistelligen Raum-Code ein.";
		
		return std::nullopt;
	}
	void JoinRoomState::sendRequest(std::string username, Avatar avatar, RoomCode roomCode) {
		this->createdGameFacade = std::make_shared<JoinRoomNetworkGameFacade>(networkErrorHandler, std::make_unique<RoomLeaveHandlerImpl>(stateManager), std::make_unique<NetworkReturnBackToMenuHandler>(stateManager), username, avatar, roomCode);
		stateManager.setGameFacade(createdGameFacade);
	}
	void JoinRoomState::handleResponseIfAvailable() {		
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
}