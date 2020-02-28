#pragma once

#include <egui/themes/AbstractTheme.h>

namespace egui {
	class MauMauTheme : public AbstractTheme {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static float const BTN_HEIGHT;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			MauMauTheme(EGuiContext& ctx);


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::function<void(Node* const)> button_coloredButton_apply();
			std::function<void(Node* const)> button_keySelectButton_apply();
			std::function<void(Node* const)> inputField_apply();
			std::function<void(Node* const)> integerSelector_apply();

	};
}