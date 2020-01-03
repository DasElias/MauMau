#pragma once
#include "State.h"
#include "../gui/MainMenuElement.h"
#include <egui/model/scene/Scene.h>
#include <shared/model/RoomCode.h>
#include "../model/JoinRoomNetworkGameFacade.h"

namespace card {
	class JoinRoomState : public State {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::MasterRenderer& eguiRenderer;
			NetworkErrorHandler& networkErrorHandler;

			std::shared_ptr<JoinRoomNetworkGameFacade> createdGameFacade = {nullptr};
			bool wasResponseReceived = false;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			JoinRoomState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer, NetworkErrorHandler& networkErrorHandler);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;
			void onStateExit() override;

		private:
			void sendRequest(std::string username, RoomCode roomCode);
			void handleResponseIfAvailable();
	};
}