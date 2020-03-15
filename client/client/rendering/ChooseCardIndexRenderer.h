#pragma once
#include <functional>
#include <optional>
#include <shared/model/CardIndex.h>

#include <egui/model/scene/Scene.h>
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodes/Button.h>
#include <egui/model/nodes/HBox.h>
#include <egui/model/nodes/Label.h>
#include "../renderingModel/CardIndexTextures.h"

namespace card {
	class ChooseCardIndexRenderer {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::MasterRenderer& eguiRenderer;
			std::function<void(CardIndex)> chooseIndexFunction;

			std::vector<std::shared_ptr<egui::Button>> indexButtons;
			std::shared_ptr<egui::HBox> buttonBar;
			std::shared_ptr<egui::Label> buttonBarBackground;
			egui::Scene scene;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ChooseCardIndexRenderer(egui::MasterRenderer& eguiRenderer, CardIndexTextures& cardIndexTextures, std::function<void(CardIndex)> chooseIndexFunction);
			ChooseCardIndexRenderer(const ChooseCardIndexRenderer&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void discardPreviousMouseEvents();
			void render();

		private:
			void adjustButtonBar();

	};
}