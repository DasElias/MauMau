#pragma once
#include "BasicRoomCreationElement.h"
#include "LabeledInputField.h"
#include "AvatarChooser.h"
#include "OptionsElement.h"

namespace card {
	class CreateLocalRoomElement : public BasicRoomCreationElement {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<AvatarChooser> avatarChooser;
			std::shared_ptr<OptionsElement> optionsElement;
			std::shared_ptr<LabeledInputField> usernameInputField;
			std::shared_ptr<LabeledInputField> amountOfOpponentsInputField;

			RoomOptions options;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreateLocalRoomElement(AvatarTextures& avatarTextures, std::size_t maxFieldLength);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getUsernameInput() const;
			std::string getAmountOfOpponentsInput() const;
			Avatar getSelectedAvatar() const;
			RoomOptions getOptions() const;
	};
}