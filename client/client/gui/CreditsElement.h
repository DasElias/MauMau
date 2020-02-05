#pragma once
#include <memory>
#include "MenuElement.h"
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodes/Button.h>
#include <egui/model/nodes/AspectRatioElement.h>
#include <egui/model/nodes/VBox.h>
#include <egui/model/nodes/HBox.h>
#include <egui/model/nodes/Label.h>
#include "ColoredButton.h"

namespace card {
	class CreditsElement : public MenuElement {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<egui::Label> content;
			std::shared_ptr<ColoredButton> backButton;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreditsElement();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void setOnBackBtn(egui::FunctionWrapper<egui::ActionEvent> handler);

		private:
			void appendContent(std::string title, std::string content);
	};
}