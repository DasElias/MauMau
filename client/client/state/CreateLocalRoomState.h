#pragma once
#include "State.h"
#include "../gui/MainMenuElement.h"
#include <egui/model/scene/Scene.h>
#include <shared/model/RoomCode.h>
#include "../model/LocalGameFacade.h"

namespace card {
	class CreateLocalRoomState : public State {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::MasterRenderer& eguiRenderer;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreateLocalRoomState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;
			void onStateExit() override;
	};
}