#pragma once
#include "State.h"
#include "../rendering/CardSceneRenderer.h"
#include "../renderingModel/ProjectionMatrix.h"
#include "../renderingModel/Viewport.h"
#include <egui/rendering/MasterRenderer.h>

namespace card {
	class IngameState : public State {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ProjectionMatrix projectionMatrix;
			Viewport viewport;
			egui::MasterRenderer& eguiRenderer;
			CardSceneRenderer sceneRenderer;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			IngameState(StateManager& stateManager, egui::MasterRenderer& eguiRenderer);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:		
			void updateAndRender(float delta) override;
			void onStateEnter() override;
			void onStateExit() override;

	};
}