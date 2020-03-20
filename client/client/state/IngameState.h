#pragma once
#include "State.h"
#include "../rendering/CardSceneRenderer.h"
#include "../renderingModel/ProjectionMatrix.h"
#include "../renderingModel/Viewport.h"
#include "../renderingModel/WorldToScreenConverter.h"
#include <egui/rendering/MasterRenderer.h>

namespace card {
	class IngameState : public State {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static const long long GAME_HASNT_ENDED = -1;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ProjectionMatrix projectionMatrix;
			Viewport viewport;
			WorldToScreenConverter worldToScreenConverter;
			egui::MasterRenderer& eguiRenderer;
			CardSceneRenderer sceneRenderer;
			long long unixTimeGameHasEnded;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			IngameState(StateManager& stateManager, AvatarTextures& avatarTextures, egui::MasterRenderer& eguiRenderer);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:		
			void updateAndRender(float delta) override;
			void onStateEnter() override;
			void onStateExit() override;

		private:
			float getYOnScreenOfMiddleOfHandCards();

	};
}