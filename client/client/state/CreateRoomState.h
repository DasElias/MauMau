#pragma once
#include "State.h"
#include "../gui/MainMenuElement.h"
#include <egui/model/scene/Scene.h>
#include <shared/model/RoomCode.h>
#include "../model/CreateRoomNetworkGameFacade.h"
#include "../gui/CreateOnlineRoomElement.h"

namespace card {
	class CreateRoomState : public State {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::size_t const USERNAME_MAX_LENGTH = 20;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::MasterRenderer& eguiRenderer;
			NetworkErrorHandler& networkErrorHandler;

			std::shared_ptr<CreateRoomNetworkGameFacade> createdGameFacade = {nullptr};

			std::shared_ptr<CreateOnlineRoomElement> element;
			egui::Scene scene;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreateRoomState(StateManager& stateManager, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer, NetworkErrorHandler& networkErrorHandler);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;

		private:
			std::optional<std::string> getLocalVerificationErrorMessage();
			void sendRequest(std::string username, Avatar avatar, RoomOptions options);
			void handleResponseIfAvailable();
	};
}