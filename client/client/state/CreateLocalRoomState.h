#pragma once
#include "State.h"
#include "../gui/MainMenuElement.h"
#include <egui/model/scene/Scene.h>
#include <shared/model/RoomCode.h>
#include "../model/LocalGameFacade.h"
#include "../gui/CreateLocalRoomElement.h"
#include <optional>

namespace card {
	class CreateLocalRoomState : public State {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::size_t const FIELDS_MAX_LENGTH = 20;
			static int const MAX_OPPONENTS = 3;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::MasterRenderer& eguiRenderer;
			std::shared_ptr<CreateLocalRoomElement> element;
			egui::Scene scene;

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

		private:
			std::optional<std::string> getErrorMessage();
	};
}