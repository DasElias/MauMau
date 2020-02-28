#include "CreateLocalRoomState.h"
#include "CreateLocalRoomState.h"
#include <egui/model/popups/PopupErrorBox.h>
#include <shared/model/MaxParticipants.h>
#include "RoomLeaveHandlerImpl.h"
#include "LocalGameEndHandler.h"

namespace card {
	int const CreateLocalRoomState::MAX_OPPONENTS = MAX_PARTICIPANTS - 1;

	CreateLocalRoomState::CreateLocalRoomState(StateManager& stateManager, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer) :
			State(stateManager),
			eguiRenderer(eguiRenderer),
			element(std::make_shared<CreateLocalRoomElement>(avatarTextures, FIELDS_MAX_LENGTH)),
			scene(element) {

		element->addBackBtnEventHandler({[this, &stateManager](egui::ActionEvent&) {
			stateManager.changeState("MainMenuState");
		
		}});
		element->addContinueBtnEventHandler({[this, &stateManager](egui::ActionEvent&) {
			std::optional<std::string> errorMsg = getErrorMessage();
			if(errorMsg.has_value()) {
				egui::PopupErrorBox eb("Lokaler Raum erstellen", *errorMsg);
				eb.show();
			} else {
				// we have verified the input before
				std::string username = element->getUsernameInput();
				int amountOfOpponents = element->getAmountOfOpponents();
				Avatar avatar = element->getSelectedAvatar();
				RoomOptions options = element->getOptions();

				auto gameFacade = std::make_shared<LocalGameFacade>(username, amountOfOpponents, avatar, options, std::make_unique<RoomLeaveHandlerImpl>(stateManager), std::make_unique<LocalGameEndHandler>(stateManager));
				stateManager.setGameFacade(gameFacade);
				gameFacade->getRoom().requestGameStart();
				stateManager.changeState("IngameState");
			}
		}});
	}
	void CreateLocalRoomState::updateAndRender(float delta) {
		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}
	void CreateLocalRoomState::onStateEnter() {
		State::onStateEnter();
		scene.discardMouseEvents();
	}
	std::optional<std::string> CreateLocalRoomState::getErrorMessage() {
		std::string usernameInput = element->getUsernameInput();

		if(usernameInput.size() > FIELDS_MAX_LENGTH) return "Dein Nutzername darf maximal " + std::to_string(FIELDS_MAX_LENGTH) + " Zeichen lang sein.";
		if(usernameInput.size() == 0) return "Dein Nutzername darf nicht nicht leer sein.";

		return std::nullopt;
	}
}