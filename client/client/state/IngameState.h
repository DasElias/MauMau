#pragma once
#include "State.h"
#include "../rendering/CardSceneRendererWithBlur.h"
#include "../rendering/PlayerLabelPositionGenerator.h"
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
			static int const FOV;
			static float const START_VIEWPORT_Z;
			static int const PRESS_ESC_DELAY = 250;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ProjectionMatrix projectionMatrix;
			Viewport viewport;
			WorldToScreenConverter worldToScreenConverter;
			PlayerLabelPositionGenerator playerLabelPositionGenerator;
			egui::MasterRenderer& eguiRenderer;
			CardSceneRenderer sceneRenderer;
			CardSceneRendererWithBlur sceneRendererWrapper;
			bool isInPauseState;
			long long unixTimePauseStateWasToggled;

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
			void updatePauseState();
			void updateProjection();
			void updateViewportY();
			void updateViewportZ();
			float getYOnScreenOfMiddleOfHandCards();
			float getTopYOfVisAVisPlayerLabel();


	};
}