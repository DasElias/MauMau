#pragma once
#include <egui/model/nodes/InputField.h>


namespace card {
	class UsernameInputFieldCharacterFilter {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static std::size_t const USERNAME_MAX_LENGTH = 20;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<egui::InputField> inputField;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			UsernameInputFieldCharacterFilter(std::shared_ptr<egui::InputField> inputField);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool operator()(unsigned int enteredChar) const;

		private:
			bool isCharacterValid(unsigned int enteredChar) const;
	};
}