#pragma once
#include "State.h"
#include "../gui/CreditsElement.h"
#include <egui/model/scene/Scene.h>

namespace card {
	class CreditsState : public State {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<CreditsElement> element;
			egui::Scene scene;
			egui::MasterRenderer& renderer;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreditsState(StateManager& stateManager, egui::MasterRenderer& renderer);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;

	};
}