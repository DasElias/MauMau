#pragma once

#include "State.h"
#include <egui/rendering/MasterRenderer.h>
#include "../gui/ParticipantsOverviewElement.h"
#include <egui/model/scene/Scene.h>

namespace card {
	class ParticipantsOverviewState : public State {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::MasterRenderer& eguiRenderer;
			std::shared_ptr<ParticipantsOverviewElement> element;
			egui::Scene scene;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ParticipantsOverviewState(StateManager& stateManager, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;

		private:
			ProxyRoom& getRoom();

	};
}