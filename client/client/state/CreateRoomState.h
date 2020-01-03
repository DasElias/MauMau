#pragma once
#include "State.h"
#include "../gui/MainMenuElement.h"
#include <egui/model/scene/Scene.h>
#include <shared/model/RoomCode.h>
#include "../model/CreateRoomNetworkGameFacade.h"

namespace card {
	class CreateRoomState : public State {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::MasterRenderer& eguiRenderer;
			NetworkErrorHandler& networkErrorHandler;

			std::shared_ptr<CreateRoomNetworkGameFacade> createdGameFacade = {nullptr};
			bool wasResponseReceived = false;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreateRoomState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer, NetworkErrorHandler& networkErrorHandler);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;
			void onStateExit() override;

		private:
			void sendRequest(std::string username);
			void handleResponseIfAvailable();
	};
}