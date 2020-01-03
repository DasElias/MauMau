#pragma once
#include <memory>
#include "MenuElement.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodes/Button.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/nodes/HBox.h>

namespace card {
	class MainMenuElement : public MenuElement {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<egui::AspectRatioElement> singlePlayerBtn;
			std::shared_ptr<egui::AspectRatioElement> createOnlineRoomBtn;
			std::shared_ptr<egui::AspectRatioElement> joinOnlineRoomBtn;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			MainMenuElement();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void addOnSinglePlayerBtnHandler(egui::FunctionWrapper<egui::ActionEvent> callback);
			void addOnCreateOnlineRoomHandler(egui::FunctionWrapper<egui::ActionEvent> callback);
			void addOnJoinOnlineRoomHandler(egui::FunctionWrapper<egui::ActionEvent> callback);

		private:
			egui::FunctionWrapper<egui::MouseEvent> convertEventHandler(egui::FunctionWrapper<egui::ActionEvent> wrapped);
	};
}