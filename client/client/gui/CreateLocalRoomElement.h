#pragma once
#include "BasicRoomJoinElement.h"
#include "JoinRoomButtonBar.h"
#include "LabeledInputField.h"

namespace card {
	class CreateLocalRoomElement : public BasicRoomJoinElement {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<LabeledInputField> usernameInputField;
			std::shared_ptr<LabeledInputField> amountOfOpponentsInputField;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreateLocalRoomElement(std::size_t maxFieldLength);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getUsernameInput() const;
			std::string getAmountOfOpponentsInput() const;
	};
}