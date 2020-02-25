#pragma once
#include "State.h"
#include "../gui/MainMenuElement.h"
#include <egui/model/scene/Scene.h>
#include <shared/model/RoomCode.h>
#include "../model/JoinRoomNetworkGameFacade.h"
#include "../gui/JoinOnlineRoomElement.h"


namespace card {
	class JoinRoomState : public State {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::size_t const USERNAME_MAX_LENGTH = 20;
			static std::size_t const ROOM_CODE_LENGTH = 3;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::MasterRenderer& eguiRenderer;
			NetworkErrorHandler& networkErrorHandler;

			std::shared_ptr<JoinRoomNetworkGameFacade> createdGameFacade = {nullptr};

			std::shared_ptr<JoinOnlineRoomElement> element;
			egui::Scene scene;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			JoinRoomState(StateManager& stateManager, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer, NetworkErrorHandler& networkErrorHandler);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;

		private:
			std::optional<std::string> getLocalVerificationErrorMessage();
			void sendRequest(std::string username, Avatar avatar, RoomCode roomCode);
			void handleResponseIfAvailable();
	};
}